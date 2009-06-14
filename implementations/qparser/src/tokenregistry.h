#ifndef __QPARSER_TOKENREGISTRY_H__
#define __QPARSER_TOKENREGISTRY_H__
//////////////////////////////////////////////////////////////////////////////
//
//    TOKENREGISTRY.H
//
//    Copyright © 2009, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      Create, manage and store parse tokens
*/
namespace QParser
{
  class TokenRegistry
  {
  public:
    // Container types
    //typedef std::unordered_map<const std::string, ParseToken> TokenValues; // Map names to token values
    typedef std::unordered_map<std::string, ParseToken> TokenValues; // Map names to token values
    typedef std::map<ParseToken, std::string> TokenNames;                  // Map token values to their corresponding names
    
    // Construction
    INLINE TokenRegistry();
    INLINE TokenRegistry(TokenRegistry&) = delete;
    
    // Look up the next available terminal token
    INLINE ParseToken GetNextAvailableTerminal() const;

    // Look up the next available nonterminal token
    INLINE ParseToken GetNextAvailableNonterminal() const;
    
    // Look up the next available temporary nonterminal id
    INLINE ParseToken GetNextAvailableTemporaryToken() const;
    
    // Add a new terminal token to the registry
    INLINE ParseToken GenerateTerminal(const_cstring terminalName);

    // Add a new non-terminal token to the registry
    INLINE ParseToken GenerateNonterminal(const_cstring nonterminalName);
    
    // Add a new temporary token to the registry
    INLINE ParseToken GenerateTemporaryToken(const_cstring nonterminalName);
    
    // Resolve a temporary token with a nonterminal token and replace the references to it in the token registry
    INLINE ParseToken ResolveTemporaryToken(const_cstring nonterminalName);
    
    // Get a token by the name used to identify it
    INLINE ParseToken GetToken(const_cstring tokenName) const;
    
    // Get a terminal token by the name used to identify it
    INLINE ParseToken GetTerminal(const_cstring terminalName) const;
    
    // Get a non-terminal token by the name used to identify it
    INLINE ParseToken GetNonterminal(const_cstring nonterminalName) const;
            
    // Get the name corresponding to a specific token by token value
    INLINE const std::string& GetTokenName(ParseToken token) const;
    
    // Get an existing nonterminal token by the name used to identify it 
    // or otherwise generate a temporary token corresponding to the name.
    INLINE ParseToken FindOrGenerateTemporaryNonterminal(const_cstring tokenName);
    
    // Test whether a token is a terminal token (as opposed to a non-terminal)
    static INLINE bool IsTerminal(ParseToken token);
    
    // Test whether a token is a nonterminal token (as opposed to a terminal)
    static INLINE bool IsNonterminal(ParseToken token);
    
    // Test whether a token is a temporary token
    INLINE bool IsTemporaryToken(ParseToken token);
    
    // Check whether a token is valid (this is mostly for debugging purposes)
    INLINE bool IsTokenValid(ParseToken token) const;
    
    // Check whether a terminal token is valid (this is mostly for debugging purposes)
    INLINE bool IsTerminalValid(ParseToken terminal) const;
    
    // Check whether a non-terminal token is valid (this is mostly for debugging purposes)
    INLINE bool IsNonterminalValid(ParseToken nonterminal) const;
    
  protected:    
    ParseToken nextTerminalToken;     // The next available terminal token
    ParseToken nextNonterminalToken;  // The next available nonterminal token
    ParseToken nextTemporaryToken;    // The next available temporary (dummy) token
    
    TokenNames terminalNames;       // Map of all terminal names corresponding to their token values
    TokenNames nonterminalNames;    // Map of all non-terminal names corresponding to their token values
    TokenValues terminalTokens;     // Map of all terminal tokens corresponding to their names
    TokenValues nonterminalTokens;  // Map of all non-terminal tokens corresponding to their names
  };
}

/*                                   INCLUDES                               */
#include "tokenregistry.inl"

#endif
