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
--import Foreign.C.Types

{-                                 CONSTANTS                                -}
-- #define PARSER_VERSION "0.1"
-- #define PARSER_TOKEN_VALUE_EOF "\x01"

{-                                   TYPES                                  -}

-- #ifdef __cplusplus
-- extern "C" {
-- #endif

{-                                     API                                  -}
  
foreign import ccall "semanticdb.h semanticDBInit"
     c_semanticDBInit :: IO ()

-- #ifdef __cplusplus
-- }
-- #endif

-- #endif
