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

{-                                   TYPES                                  -}
type SemanticId = CUInt
type OSint16 = CShort
type OSint32 = CInt
type OSint64 = CLLong
type OSuint16 = CUShort
type OSuint32 = CUInt
type OSuint64 = CULLong
type OSfloat = CFloat
type OSdouble = CDouble
type OSobject = OSuint32

type QueryFunction = IO ()

{-                                 CONSTANTS                                -}
c_SEMANTICID_INVALID  :: SemanticId
c_SEMANTICID_EPSILON  :: SemanticId
c_SEMANTICID_QUERY    :: SemanticId
c_SEMANTICID_INT16    :: SemanticId
c_SEMANTICID_INT32    :: SemanticId
c_SEMANTICID_INT64    :: SemanticId
c_SEMANTICID_UINT16   :: SemanticId
c_SEMANTICID_UINT32   :: SemanticId
c_SEMANTICID_UINT64   :: SemanticId
c_SEMANTICID_FLOAT    :: SemanticId
c_SEMANTICID_DOUBLE   :: SemanticId
c_SEMANTICID_CHAR     :: SemanticId
c_SEMANTICID_STRING   :: SemanticId

c_SEMANTICID_INVALID  = 0xffffffff
c_SEMANTICID_EPSILON  = 0xfffffffe
c_SEMANTICID_QUERY    = 0xfffffffd
c_SEMANTICID_INT16    = 0xfffffffc
c_SEMANTICID_INT32    = 0xfffffffb
c_SEMANTICID_INT64    = 0xfffffffa
c_SEMANTICID_UINT16   = 0xfffffff9
c_SEMANTICID_UINT32   = 0xfffffff8
c_SEMANTICID_UINT64   = 0xfffffff7
c_SEMANTICID_FLOAT    = 0xfffffff6
c_SEMANTICID_DOUBLE   = 0xfffffff5
c_SEMANTICID_CHAR     = 0xfffffff4
c_SEMANTICID_STRING   = 0xfffffff3

{-                                    API                                   -}
semanticDBInit :: IO (StablePtr (Maybe Int))

{-                               DEBUGING API                               -}

{-                                   C API                                  -}
{- symbols -}
foreign import ccall unsafe "semanticdb.h DeclareSymbol"
  c_DeclareSymbol :: CString -> IO SemanticId

foreign import ccall unsafe "semanticdb.h GlobalSymbol"
  c_GlobalSymbol :: CString -> IO SemanticId

foreign import ccall unsafe "semanticdb.h AnonymousSymbol"
  c_AnonymousSymbol :: IO SemanticId

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

foreign import ccall unsafe "semanticdb.h OpenDomain"
  c_OpenDomain :: SemanticId -> IO ()

foreign import ccall unsafe "semanticdb.h OpenHiddenDomain"
  c_OpenHiddenDomain :: SemanticId -> IO ()

foreign import ccall unsafe "semanticdb.h CloseDomain"
  c_CloseDomain :: SemanticId -> IO ()

{-- queries --}
foreign import ccall unsafe "semanticdb.h BeginQuery"
  c_BeginQuery :: IO ()

foreign import ccall unsafe "semanticdb.h EndQuery"
  c_EndQuery :: IO SemanticId

foreign import ccall unsafe "semanticdb.h SelectionDisjunct"
  c_SelectionDisjunct :: QueryFunction

foreign import ccall unsafe "semanticdb.h SelectionExclusiveDisjunct"
  c_SelectionExclusiveDisjunct :: QueryFunction

foreign import ccall unsafe "semanticdb.h SelectionConjunct"
  c_SelectionConjunct :: QueryFunction

foreign import ccall unsafe "semanticdb.h SelectionStrictConjunct"
  c_SelectionStrictConjunct :: QueryFunction

--foreign import ccall unsafe "semanticdb.h SelectionStrictExclusiveDisjunct"
--  c_SelectionStrictExclusiveDisjunct :: QueryFunction

foreign import ccall unsafe "semanticdb.h MutationDisjunct"
  c_MutationDisjunct :: QueryFunction

foreign import ccall unsafe "semanticdb.h MutationExclusiveDisjunct"
  c_MutationExclusiveDisjunct :: QueryFunction

foreign import ccall unsafe "semanticdb.h MutationConjunct"
  c_MutationConjunct :: QueryFunction

foreign import ccall unsafe "semanticdb.h MutationStrictConjunct"
  c_MutationStrictConjunct :: QueryFunction

--foreign import ccall unsafe "semanticdb.h MutationStrictExclusiveDisjunct"
--  c_MutationStrictExclusiveDisjunct :: QueryFunction

{- evaluation -}
foreign import ccall unsafe "semanticdb.h BeginEvaluation"
  c_BeginEvaluation :: SemanticId -> IO OSobject

foreign import ccall unsafe "semanticdb.h Eval"
  c_Eval :: IO SemanticId

foreign import ccall unsafe "semanticdb.h GetEvalDomain"
  c_GetEvalDomain :: IO SemanticId

foreign import ccall unsafe "semanticdb.h GetEvalString"
  c_GetEvalString :: IO CString

foreign import ccall unsafe "semanticdb.h GetEvalInt16"
  c_GetEvalInt16 :: IO OSint16

foreign import ccall unsafe "semanticdb.h GetEvalInt32"
  c_GetEvalInt32 :: IO OSint32

foreign import ccall unsafe "semanticdb.h GetEvalInt64"
  c_GetEvalInt64 :: IO OSint64

foreign import ccall unsafe "semanticdb.h GetEvalFloat"
  c_GetEvalFloat :: IO OSfloat

foreign import ccall unsafe "semanticdb.h GetEvalDouble"
  c_GetEvalDouble :: IO OSdouble

foreign import ccall unsafe "semanticdb.h EndEvaluation"
  c_EndEvaluation :: OSobject -> IO ()

foreign import ccall unsafe "semanticdb.h GetDomain"
  c_GetDomain :: SemanticId -> IO SemanticId

foreign import ccall unsafe "semanticdb.h GetString"
  c_GetString :: SemanticId -> IO CString

foreign import ccall unsafe "semanticdb.h GetInt16"
  c_GetInt16 :: SemanticId -> IO OSint16

foreign import ccall unsafe "semanticdb.h GetInt32"
  c_GetInt32 :: SemanticId -> IO OSint32

foreign import ccall unsafe "semanticdb.h GetInt64"
  c_GetInt64 :: SemanticId -> IO OSint64

foreign import ccall unsafe "semanticdb.h GetFloat"
  c_GetFloat :: SemanticId -> IO OSfloat

foreign import ccall unsafe "semanticdb.h GetDouble"
  c_GetDouble :: SemanticId -> IO OSdouble

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

