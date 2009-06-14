#ifdef  __QPARSER_TOKENREGISTRY_H__
#ifndef __QPARSER_TOKENREGISTRY_INL__
#define __QPARSER_TOKENREGISTRY_INL__
//////////////////////////////////////////////////////////////////////////////
//
//    TOKENREGISTRY.INL
//
//    Copyright © 2009, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

namespace QParser
{
  INLINE TokenRegistry::TokenRegistry() :
    nextTerminalToken(TOKEN_TERMINAL_FIRST),
    nextNonterminalToken(TOKEN_NONTERMINAL_FIRST),
    nextTemporaryToken(~1)
  {
  }
    
  INLINE ParseToken TokenRegistry::GetNextAvailableTerminal() const
  {
    return nextTerminalToken;
  }

  INLINE ParseToken TokenRegistry::GetNextAvailableNonterminal() const
  {
    return nextNonterminalToken;
  }
  
  INLINE ParseToken TokenRegistry::GetNextAvailableTemporaryToken() const
  {
    return nextTemporaryToken;
  }
  
  INLINE ParseToken TokenRegistry::GenerateTerminal(const_cstring terminalName)
  {
    // Try to find an existing terminal token with this name
    TokenValues::const_iterator i = terminalTokens.find(terminalName);
    if(i != terminalTokens.end())
      return i->second;
    
    // Generate a new token for this terminal
    ParseToken token = nextTerminalToken;
    ++nextTerminalToken;
    
    // Insert the terminal into the token name registries
    terminalTokens[terminalName] = token;
    terminalNames[token] = terminalName;
    
    return token;
  }

  INLINE ParseToken TokenRegistry::GenerateNonterminal(const_cstring nonterminalName)
  {
    // Try to find an existing terminal token with this name
    TokenValues::const_iterator i = nonterminalTokens.find(nonterminalName);
    if(i != nonterminalTokens.end() && !IsTemporaryToken(i->second))
      return i->second;
    
    // Generate a new token for this nonterminal
    ParseToken token = nextNonterminalToken;
    ++nextNonterminalToken;
    
    // If token is in the range of dummy tokens, this will not work. 
    // (Then we've run out of token values to use for forward declarations and we'll need to think of a new strategy...)
    OSI_ASSERT(token <= nextTemporaryToken);
    
    // Insert the nonterminal into the token name registries
    nonterminalTokens[nonterminalName] = token;
    nonterminalNames[token] = nonterminalName;
    
    return token;
  }
  
  INLINE ParseToken TokenRegistry::GenerateTemporaryToken(const_cstring nonterminalName)
  {
    ParseToken token = nextTemporaryToken;
    nextTemporaryToken--;
    
    // If the temporary token is in the range of valid nonterminal tokens, this will not work. 
    // (Then we've run out of token values to use for forward declarations and we'll need to think of a new strategy...)
    OSI_ASSERT(token >= nextNonterminalToken);
    
    // Insert the nonterminal into the token name registries
    // (don't add it to the nonterminalNames yet: it will be added when the token is resolved to a nonterminal)
    nonterminalTokens[nonterminalName] = token;    
                  
    return token;
  }
  
  INLINE ParseToken TokenRegistry::ResolveTemporaryToken(const_cstring nonterminalName)
  {
    // Generate a new nonterminal corresponding to the temporary token's name
    // This automatically replaces its entries in the registry (see GenerateTemporaryToken, GenerateNonterminal)
    return GenerateNonterminal(nonterminalName);
  }
  
  INLINE ParseToken TokenRegistry::GetToken(const_cstring tokenName) const
  {
    // Try to find a terminal token with this name
    TokenValues::const_iterator i = terminalTokens.find(tokenName);
    if(i != terminalTokens.end())
      return i->second;

    // Try to find a non-terminal token with this name
    i = nonterminalTokens.find(tokenName);
    if(i != nonterminalTokens.end())
      return i->second;
    
    return ParseToken(-1);  // no matching token found
  }
  
  INLINE ParseToken TokenRegistry::GetTerminal(const_cstring terminalName) const
  {
    // Try to find a terminal token with this name
    TokenValues::const_iterator i = terminalTokens.find(terminalName);
    if(i != terminalTokens.end())
      return i->second;
    
    return ParseToken(-1); // no matching terminal token found
  }
    
  INLINE ParseToken TokenRegistry::GetNonterminal(const_cstring nonterminalName) const
  {
    // Try to find a non-terminal token with this name
    TokenValues::const_iterator i = nonterminalTokens.find(nonterminalName);
    if(i != nonterminalTokens.end())
      return i->second;
    
    return ParseToken(-1);  // no matching non-terminal token found
  }
  
  INLINE const std::string& TokenRegistry::GetTokenName(ParseToken token) const
  {
    using std::string;
    static const string tokenUnknownTerminal("[Unknown Terminal]");
    static const string tokenUnknownNonterminal("[Unknown Nonterminal]");
    static const string tokenEOF("[Special (EOF)]");
    static const string tokenIdentifier("[Identifier]");
    //static const string tokenIdentifierDecl("[Identifier Decl]");
    //static const string tokenIdentifierRef("[Identifier Ref]");
    static const string tokenLiteral("[Literal]");

    // Lookup terminal token names
    if(IsTerminal(token))
    {
      switch(token)
      {
      case TOKEN_TERMINAL_IDENTIFIER: return tokenIdentifier;
      case TOKEN_TERMINAL_LITERAL:    return tokenLiteral;
      case TOKEN_SPECIAL_EOF:         return tokenEOF;
      //case ID_IDENTIFIER_DECL:      return tokenIdentifierDecl;
      //case ID_IDENTIFIER_REF:       return tokenIdentifierRef;
      }

      TokenNames::const_iterator i = terminalNames.find(token);
      if(i != terminalNames.end() && i->first == token)
        return i->second;
      
      return tokenUnknownTerminal;
    }
    
    // Look up non-terminal token names
    TokenNames::const_iterator i = nonterminalNames.find(token);
    if(i != nonterminalNames.end() && i->first == token)
      return i->second;

    return tokenUnknownNonterminal;
  }
  
  INLINE ParseToken TokenRegistry::FindOrGenerateTemporaryNonterminal(const_cstring nonterminalName)
  {
    ParseToken token = -1;
    
    // Try to find existing an production producing this token name
    TokenValues::const_iterator i = nonterminalTokens.find(nonterminalName);
    if(i == nonterminalTokens.end())
    {
      // Generate a temporary token value for this non-terminal
      token = GenerateTemporaryToken(nonterminalName);
    }
    else
    {
      // Return the existing non-terminal token
      token = i->second;
    }
    
    return token;
  }
  
  INLINE bool TokenRegistry::IsTerminal(ParseToken token)
  {
    return (token & TOKEN_FLAG_SHIFT) == TOKEN_FLAG_SHIFT;
  }
  
  INLINE bool TokenRegistry::IsNonterminal(ParseToken token)
  {
    return !IsTerminal(token);
  }
  
  INLINE bool TokenRegistry::IsTemporaryToken(ParseToken token)
  {
    return token > nextNonterminalToken;
  }
  
  INLINE bool TokenRegistry::IsTokenValid(ParseToken token) const
  {
    return IsTerminal(token)? token < nextTerminalToken : token < nextNonterminalToken;
  }
  
  INLINE bool TokenRegistry::IsTerminalValid(ParseToken terminal) const
  {
    return IsTerminal(terminal) && terminal < nextTerminalToken;
  }
  
  INLINE bool TokenRegistry::IsNonterminalValid(ParseToken nonterminal) const
  {
    return !IsTerminal(nonterminal) && nonterminal < nextNonterminalToken;
  }
}

#endif
#endif
