if exists("b:current_syntax")
  finish
endif


syn keyword pminkcfgFields TYPES CONFIG
syn keyword pminkcfgConsts METHOD SCRIPT CONST
syn keyword pminkcfgConsts PTRN contained
syn match pminkcfgOp "\[" contained
syn match pminkcfgOp "\]" contained
syn match pminkcfgOp "{"
syn match pminkcfgConsts "*"
syn match pminkcfgConsts "/S/"
syn match pminkcfgOp "}"
syn match pminkcfgComment2 "//.*$"
syn match pminkcfgString '".\{-}"'
syn match pminkcfgRegex "PTRN.\{-}PTRN" contains=pminkcfgConsts
syn match pminkcfgRegex "\[.\{-}\]" contains=pminkcfgOp
syn region pminkcfgComment2 start="/\*" end="\*/"

let b:current_syntax = "pminkcfg"

hi def link pminkcfgComment2 Comment
hi def link pminkcfgKeywords Constant
hi def link pminkcfgString Constant
hi def link pminkcfgConsts Type
hi def link pminkcfgFields Statement
hi def link pminkcfgActions Constant
hi def link pminkcfgNodeTypes PreProc
hi def link pminkcfgOp PreProc
hi def link pminkcfgRegex PreProc

hi def link pminkcfgExpr Type
hi def link pminkcfgEvalTrue Type
hi def link pminkcfgEvalFalse Type

hi def link pminkcfgAttr Comment
hi def link pminkcfgStmt Type
hi def link pminkcfgColon1 Type
hi def link pminkcfgColon2 Type


