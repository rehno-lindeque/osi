------------------------------------------------------------------------------
--
--    SEMANTICDB.HS
--
--    Ver 0.1 (2009-08-21)
--
--    Copyright © 2009, Rehno Lindeque. All rights reserved.
--
------------------------------------------------------------------------------
{-# LANGUAGE ForeignFunctionInterface #-}
module OSIX.SemanticDB where

{-                               DOCUMENTATION                              -}
{-
    DESCRIPTION:
      OpenSemanticDB Haskell FFI interface.
-}
{-                                 INCLUDES                                 -}
import Foreign
import Foreign.C
import Foreign.C.Types

{-                                 CONSTANTS                                -}

{-                                   TYPES                                  -}
type SemanticId = CUInt

{-                                    API                                   -}
semanticDBInit :: IO (StablePtr (Maybe Int))

{-                               DEBUGING API                               -}

{-                                   C API                                  -}
{- symbols -}
foreign import ccall unsafe "semanticdb.h DeclareSymbol"
  c_DeclareSymbol :: CString -> IO SemanticId

foreign import ccall unsafe "semanticdb.h GlobalSymbol"
  c_GlobalSymbol :: CString -> IO SemanticId

{- relations -}

{-- declarations --}
foreign import ccall unsafe "semanticdb.h DeclareRelation"
  c_DeclareRelation :: SemanticId -> SemanticId -> IO SemanticId

{-- queries --}
foreign import ccall unsafe "semanticdb.h SelectRelation"
  c_SelectRelation :: SemanticId -> SemanticId -> IO SemanticId

{- domains -}
foreign import ccall unsafe "semanticdb.h DeclareOpenDomain"
  c_DeclareOpenDomain :: CString -> IO SemanticId

foreign import ccall unsafe "semanticdb.h CloseDomain"
  c_CloseDomain :: CString -> IO ()

{- debuging -}
foreign import ccall unsafe "semanticdb.h SemanticDBDebugInit"
  c_SemanticDBDebugInit :: IO (StablePtr (Maybe Int))

{- initialization -}
foreign import ccall unsafe "semanticdb.h SemanticDBInit"
  c_SemanticDBInit :: IO ()

{-                              C DEBUGING API                              -}
--{-
foreign import ccall unsafe "semanticdbdbg.h DebugOutputEnvironment"
  c_DebugOutputEnvironment :: IO ()
--}

{-                              IMPLEMENTATION                              -}
semanticDBInit = c_SemanticDBInit >> c_SemanticDBDebugInit

