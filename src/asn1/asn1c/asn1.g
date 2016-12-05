
grammar asn1;
options {
	backtrack=true;
	memoize=true;
	output=AST;
	
        language=C;
        ASTLabelType=pANTLR3_BASE_TREE;
        
	
}

tokens {
	A_ROND                  =       '@';
        STAR                    =       '*';
        ASSIGN_OP               =       '::=';
        BOOLEAN_LITERAL         =       'BOOLEAN';
        TRUE_LITERAL            =       'TRUE';
        FALSE_LITERAL           =       'FALSE';
        DOT                     =       '.';
        DOUBLE_DOT              =       '..';
        ELLIPSIS                =       '...';
        APOSTROPHE              =       '\'';
        AMPERSAND               =       '&';
        LESS_THAN               =       '<';
        GREATER_THAN            =       '>';
        LESS_THAN_SLASH         =       '</';
        SHASH_GREATER_THEN      =       '/>';
        TRUE_SMALL_LITERAL      =       'true';
        FALSE_SMALL_LITERAL     =       'false';
        INTEGER_LITERAL         =       'INTEGER';
        IA5_STRING_LITERAL	=	'IA5String';
	PRINTABLE_STRING_LITERAL=	'PrintableString';
	NUMERIC_STRING_LITERAL	=	'NumericString';
        L_BRACE                 =       '{';
        R_BRACE                 =       '}';
        COMMA                   =       ',';
        L_PARAN                 =       '(';
        R_PARAN                 =       ')';
        MINUS                   =       '-';
        ENUMERATED_LITERAL      =       'ENUMERATED';
        REAL_LITERAL            =       'REAL';
        PLUS_INFINITY_LITERAL   =       'PLUS-INFINITY';
        MINUS_INFINITY_LITERAL  =       'MINUS-INFINITY';
        BIT_LITERAL             =       'BIT';
        STRING_LITERAL          =       'STRING';
        CONTAINING_LITERAL      =       'CONTAINING';
        OCTET_LITERAL           =       'OCTET';
        NULL_LITERAL            =       'NULL';
        SEQUENCE_LITERAL        =       'SEQUENCE';
        OPTIONAL_LITERAL        =       'OPTIONAL';
        DEFAULT_LITERAL         =       'DEFAULT';
        COMPONENTS_LITERAL      =       'COMPONENTS';
        OF_LITERAL              =       'OF';
        SET_LITERAL             =       'SET';
        EXCLAM                  =       '!';
        ALL_LITERAL             =       'ALL';
        EXCEPT_LITERAL          =       'EXCEPT';
        POWER                   =       '^';
        PIPE                    =       '|';
        UNION_LITERAL           =       'UNION';
        INTERSECTION_LITERAL    =       'INTERSECTION';
        INCLUDES_LITERAL        =       'INCLUDES';
        MIN_LITERAL             =       'MIN';
        MAX_LITERAL             =       'MAX';
        SIZE_LITERAL            =       'SIZE';
        FROM_LITERAL            =       'FROM';
        WITH_LITERAL            =       'WITH';
        COMPONENT_LITERAL       =       'COMPONENT';
        PRESENT_LITERAL         =       'PRESENT';
        ABSENT_LITERAL          =       'ABSENT';
        PATTERN_LITERAL         =       'PATTERN';
        TYPE_IDENTIFIER_LITERAL =       'TYPE-Identifier';
        ABSTRACT_SYNTAX_LITERAL =       'ABSTRACT-SYNTAX';
       	CLASS_LITERAL           =       'CLASS';
        UNIQUE_LITERAL          =       'UNIQUE';
        SYNTAX_LITERAL          =       'SYNTAX';
        L_BRACKET               =       '[';
        R_BRACKET               =       ']';
        INSTANCE_LITERAL        =       'INSTANCE';
        SEMI_COLON              =       ';';
        IMPORTS_LITERAL         =       'IMPORTS';
        EXPORTS_LITERAL         =       'EXPORTS';
        EXTENSIBILITY_LITERAL   =       'EXTENSIBILITY';
        IMPLIED_LITERAL         =       'IMPLIED';
        EXPLICIT_LITERAL        =       'EXPLICIT';
        TAGS_LITERAL            =       'TAGS';
        IMPLICIT_LITERAL        =       'IMPLICIT';
        AUTOMATIC_LITERAL       =       'AUTOMATIC';
        DEFINITIONS_LITERAL     =       'DEFINITIONS';
        BEGIN_LITERAL           =       'BEGIN';
        END_LITERAL             =       'END';
        DOUBLE_L_BRACKET        =       '[[';
        DOUBLE_R_BRACKET        =       ']]';
        COLON                   =       ':';
        CHOICE_LITERAL          =       'CHOICE';
        UNIVERSAL_LITERAL       =       'UNIVERSAL';
        APPLICATION_LITERAL     =       'APPLICATION';
        PRIVATE_LITERAL         =       'PRIVATE';
        EMBEDDED_LITERAL        =       'EMBEDDED';
        PDV_LITERAL             =       'PDV';
        //EXTERNAL_LITERAL        =       'EXTERNAL';
        OBJECT_LITERAL          =       'OBJECT';
        IDENTIFIER_LITERAL      =       'IDENTIFIER';
        RELATIVE_OID_LITERAL    =       'RELATIVE-OID';
        CHARACTER_LITERAL       =       'CHARACTER';
        CONSTRAINED_LITERAL     =       'CONSTRAINED';
        BY_LITERAL              =       'BY';
        A_ROND_DOT              =       '@.';
        ENCODED_LITERAL         =       'ENCODED';
        COMMENT                 =       '--';
        // AST
        MODULE_ROOT;
        MODULE_ID;
        MODULE_VERSION;
        DEFINITIONS_NODE;
        IDENTIFIER_NODE;
        VALUE_NODE;
        TYPE_NODE;
        DEFINITION_NODE;
        EXPORTS_NODE;
        IMPORTS_NODE;
        NODE_INFO;
        VALUE_NODE;
        BODY_NODE;
        TAG_DESCRIPTOR;
        TAG_DEFINITIONS;
        NODE_COMPONENT;
        TAG_SPECIFIC;
        NODE_NAME;
        

}

moduleDefinition :  
     id=IDENTIFIER (L_BRACE ((id2+=IDENTIFIER | id2+=NUMBER) (L_PARAN id2+=NUMBER R_PARAN)*)* R_BRACE)?
     DEFINITIONS_LITERAL
     td=tagDefault
     extensionDefault 
     ASSIGN_OP 
     BEGIN_LITERAL
     moduleBody 
     END_LITERAL 
     -> ^(MODULE_ROOT ^(MODULE_ID $id $id2*) ^(DEFINITION_NODE $td? moduleBody))
      
;
        
        
        
tagDefault : ((EXPLICIT_LITERAL|IMPLICIT_LITERAL|AUTOMATIC_LITERAL) TAGS_LITERAL! )? 
;

extensionDefault : 
   (EXTENSIBILITY_LITERAL IMPLIED_LITERAL)?
;

moduleBody :  (ex=exports im=imports as=assignmentList)? -> ^(BODY_NODE ^(EXPORTS_NODE $ex?) ^(IMPORTS_NODE $im?) ^($as))
;

exports :   (EXPORTS_LITERAL se=symbolsExported SEMI_COLON -> $se
 	|    EXPORTS_LITERAL ALL_LITERAL SEMI_COLON )?
;

symbolsExported : ( sl=symbolList )?
-> $sl
;
 
imports :   (IMPORTS_LITERAL symbolsImported SEMI_COLON )?
	-> ^(IMPORTS_LITERAL symbolsImported)?
;

symbolsImported : (symbolsFromModuleList )? 
;
 
symbolsFromModuleList : 
     (symbolsFromModule) (symbolsFromModule)* 
;

symbolsFromModule : symbolList FROM_LITERAL globalModuleReference -> symbolList ^(FROM_LITERAL globalModuleReference)
;

globalModuleReference : IDENTIFIER assignedIdentifier (L_BRACE! ((IDENTIFIER | NUMBER) (L_PARAN! NUMBER R_PARAN!)?)* R_BRACE!)?
;

assignedIdentifier : 
;

symbolList   : (smb1=symbol) (COMMA smb2+=symbol)* -> $smb1 $smb2*
;

symbol  : id=IDENTIFIER ((L_BRACE  R_BRACE))? 
-> $id
;


assignmentList :  (as1=assignment) (as2+=assignment)*
-> ^(TAG_DEFINITIONS $as1 $as2*)
;


assignment : 
 (id=IDENTIFIER
 	(  as1=valueAssignment 
	 | as2=typeAssignment
	 | as3=parameterizedAssignment 
	 | as4=objectClassAssignment
	)

 )  
 -> ^($id $as1? $as2? $as3? $as4?)
;
	
sequenceType :SEQUENCE_LITERAL L_BRACE (extensionAndException  optionalExtensionMarker | componentTypeLists )? R_BRACE 
-> ^(SEQUENCE_LITERAL extensionAndException? optionalExtensionMarker? componentTypeLists?)
;
extensionAndException :  ELLIPSIS!  exceptionSpec?
;
optionalExtensionMarker :  ( COMMA  ELLIPSIS )? 
;

componentTypeLists :   
   rootComponentTypeList (COMMA!  extensionAndException  extensionAdditions   (optionalExtensionMarker|(EXTENSTIONENDMARKER  COMMA  rootComponentTypeList)))?
  |  extensionAndException  extensionAdditions  (optionalExtensionMarker | (EXTENSTIONENDMARKER  COMMA    rootComponentTypeList))
;
rootComponentTypeList  : componentTypeList
;
componentTypeList  : (componentType) (COMMA! componentType)* 
;
componentType  	: nt1=namedType (ol=OPTIONAL_LITERAL | dl=DEFAULT_LITERAL nt2=namedType )? -> ^($nt1 $nt2? ^(TAG_DESCRIPTOR $ol? $dl?))
		|  COMPONENTS_LITERAL OF_LITERAL  type
;

extensionAdditions  :  (COMMA!  extensionAdditionList)?
;

extensionAdditionList  :  (extensionAddition) (COMMA!  extensionAddition)* 
;

extensionAddition  : componentType  |  extensionAdditionGroup 
;

extensionAdditionGroup  :  DOUBLE_L_BRACKET  versionNumber  componentTypeList  DOUBLE_R_BRACKET 
;

versionNumber  :  (NUMBER  COLON )?
;

sequenceOfType  : SEQUENCE_LITERAL sz+=SIZE_LITERAL* (L_PARAN* (c1=constraint | c2=sizeConstraint) R_PARAN*)? OF_LITERAL (tp=type | nt=namedType )
		-> ^(SEQUENCE_LITERAL["SEQUENCE_OF"] ^(TAG_SPECIFIC $sz* $c1? $c2? $tp? $nt?))
;

sizeConstraint : SIZE_LITERAL constraint 
;
	
parameterizedAssignment : 
 parameterList 
(ASSIGN_OP 
	(type 
		|	value
		|	valueSet
	)
)
|( definedObjectClass ASSIGN_OP!
	( object 
		|	objectClass
		|	objectSet
	)

)
// parameterizedTypeAssignment  
//| parameterizedValueAssignment  
//| parameterizedValueSetTypeAssignment 
//| parameterizedObjectClassAssignment 
//| parameterizedObjectAssignment 
//| parameterizedObjectSetAssignment 
;
parameterList : L_BRACE parameter (COMMA parameter)* R_BRACE 
;
parameter : (paramGovernor COLON)? IDENTIFIER
;
paramGovernor : governor | IDENTIFIER 
;
//dummyGovernor : dummyReference 
//;

governor : type | definedObjectClass 
;

	
objectClassAssignment : /*IDENTIFIER*/ ASSIGN_OP! objectClass 
;

objectClass : a1=definedObjectClass | a2=objectClassDefn | a3=value/*| parameterizedObjectClass */
	-> ^(VALUE_NODE $a1? $a2? $a3?)
;
definedObjectClass : 
	((id1=IDENTIFIER DOT)? id2=IDENTIFIER 
	| TYPE_IDENTIFIER_LITERAL
	|  ABSTRACT_SYNTAX_LITERAL        
	) (L_PARAN (c1=constraint | c2=sizeConstraint) R_PARAN)?
	-> ^(NODE_INFO $id1? ^($id2 ^(TAG_SPECIFIC $c1? $c2?)))
	
;
usefulObjectClassReference : 
   TYPE_IDENTIFIER_LITERAL
 |  ABSTRACT_SYNTAX_LITERAL 
;

externalObjectClassReference : IDENTIFIER DOT IDENTIFIER 
;

objectClassDefn : CLASS_LITERAL L_BRACE  fieldSpec (COMMA fieldSpec  )*  R_BRACE  withSyntaxSpec? 
;
withSyntaxSpec : WITH_LITERAL SYNTAX_LITERAL syntaxList 
;
syntaxList : L_BRACE tokenOrGroupSpec+ R_BRACE 
;

tokenOrGroupSpec : requiredToken | optionalGroup 
;

optionalGroup : L_BRACKET (tokenOrGroupSpec)+ R_BRACKET 
;

requiredToken : literal | primitiveFieldName 
;
literal : IDENTIFIER | COMMA 
;
primitiveFieldName :
	AMPERSAND IDENTIFIER;


fieldSpec : 
	AMPERSAND IDENTIFIER 
	(
	  typeOptionalitySpec? 	
  	| type (valueSetOptionalitySpec?  | UNIQUE_LITERAL? valueOptionalitySpec? )
	| fieldName (OPTIONAL_LITERAL | (DEFAULT_LITERAL (valueSet | value)))?
	| definedObjectClass (OPTIONAL_LITERAL | (DEFAULT_LITERAL (objectSet | object)))?

	)
	
//   typeFieldSpec 
//  | fixedTypeValueFieldSpec 
//  | variableTypeValueFieldSpec 
//  | fixedTypeValueSetFieldSpec 
//  | variableTypeValueSetFieldSpec 
//  | objectFieldSpec 
//  | objectSetFieldSpec 
;

typeFieldSpec : AMPERSAND IDENTIFIER typeOptionalitySpec? 
;
typeOptionalitySpec : OPTIONAL_LITERAL | (DEFAULT_LITERAL type)
;
fixedTypeValueFieldSpec : AMPERSAND IDENTIFIER type UNIQUE_LITERAL? valueOptionalitySpec ? 
;
valueOptionalitySpec : OPTIONAL_LITERAL | (DEFAULT_LITERAL value)
;

variableTypeValueFieldSpec : AMPERSAND IDENTIFIER  fieldName valueOptionalitySpec ? 
;

fixedTypeValueSetFieldSpec : AMPERSAND IDENTIFIER   type valueSetOptionalitySpec ? 
;

valueSetOptionalitySpec : OPTIONAL_LITERAL | DEFAULT_LITERAL valueSet 
;

object : definedObject /*| objectDefn | objectFromObject */|  parameterizedObject 
;
parameterizedObject : definedObject actualParameterList 
;


definedObject
	:	IDENTIFIER (DOT)?
	; 
objectSet : L_BRACE objectSetSpec R_BRACE 
;
objectSetSpec : 
  rootElementSetSpec (COMMA ELLIPSIS (COMMA additionalElementSetSpec )?)?
 | ELLIPSIS (COMMA additionalElementSetSpec )?
;


fieldName :(AMPERSAND IDENTIFIER)(AMPERSAND IDENTIFIER DOT)*
;
valueSet : L_BRACE elementSetSpecs R_BRACE 
;
elementSetSpecs : 
 rootElementSetSpec (COMMA ELLIPSIS (COMMA additionalElementSetSpec)?)?
	;
rootElementSetSpec : elementSetSpec 
;
additionalElementSetSpec : elementSetSpec 
;
elementSetSpec : unions | ALL_LITERAL exclusions 
;
unions :   (intersections) (unionMark intersections)*
;
exclusions : EXCEPT_LITERAL elements 
;
intersections : (intersectionElements) (intersectionMark intersectionElements)*       
;
unionMark  :  PIPE  |  UNION_LITERAL 
;

intersectionMark  :  POWER |  INTERSECTION_LITERAL 
;

elements  : subtypeElements 
// |  objectSetElements 
// |  L_PARAN elementSetSpec R_PARAN 
;
objectSetElements : 
    object | definedObject /*| objectSetFromObjects | parameterizedObjectSet      */
;


intersectionElements : elements (exclusions)?
;
subtypeElements :    
  ((value | MIN_LITERAL) LESS_THAN?  DOUBLE_DOT LESS_THAN?  (value | MAX_LITERAL) )
  |sizeConstraint
 | (PATTERN_LITERAL value)
 | value 
;


variableTypeValueSetFieldSpec : AMPERSAND IDENTIFIER    fieldName valueSetOptionalitySpec? 
;
objectFieldSpec : AMPERSAND IDENTIFIER definedObjectClass objectOptionalitySpec? 
;
objectOptionalitySpec : OPTIONAL_LITERAL | DEFAULT_LITERAL object 
;
objectSetFieldSpec : AMPERSAND IDENTIFIER definedObjectClass objectSetOptionalitySpec ? 
;
objectSetOptionalitySpec : OPTIONAL_LITERAL | DEFAULT_LITERAL objectSet 
;
	
typeAssignment : 
      ASSIGN_OP!
      type 
      //-> ^(type)
;

valueAssignment :  
	
      type     
	  ASSIGN_OP 
       value
       -> ^(type value)
;
extraTagDescriptors 
	: (L_BRACKET (id=UNIVERSAL_LITERAL | id=APPLICATION_LITERAL | id=PRIVATE_LITERAL)* num=NUMBER R_BRACKET) im=IMPLICIT_LITERAL* ex=EXPLICIT_LITERAL*
	-> ^(TAG_DESCRIPTOR $id? $im? $ex? $num)
	;
	
type : et=extraTagDescriptors* (tp=builtinType | tp2=referencedType) /*(constraint)**/
-> ^(NODE_INFO $et? $tp? $tp2?)
;
builtinType :
   octetStringType 
 | ia5stringType
 | booleanType
 | nullType
 | bitStringType
 | choiceType 
 | enumeratedType 
 | integerType 
 | printableStringType
 | numericStringType
 | sequenceType 
 | sequenceOfType 
 | setType 
 | setOfType 
 | objectidentifiertype 
 | relativeoidtype
 | objectClassFieldType 

;

objectClassFieldType : definedObjectClass DOT fieldName 
;

	
setType :  SET_LITERAL  L_BRACE  (extensionAndException  optionalExtensionMarker  | componentTypeLists)? R_BRACE 
	;
	
setOfType    : SET_LITERAL (constraint | sizeConstraint)? OF_LITERAL (type | namedType)
;

referencedType :
  dt=definedType 
// | selectionType 
// | typeFromObject 
// | valueSetFromObjects 
-> ^($dt)
;
definedType : 
id=IDENTIFIER (DOT IDENTIFIER)? (DEFAULT_LITERAL IDENTIFIER)? pl=actualParameterList? -> ^($id $pl?)
; 


constraint : L_PARAN! constraintSpec  exceptionSpec R_PARAN!
	   | constraintSpec  exceptionSpec
//L_PARAN value DOT_DOT value R_PARAN 
;

constraintSpec : generalConstraint | subtypeConstraint  
;
userDefinedConstraint : CONSTRAINED_LITERAL BY_LITERAL L_BRACE userDefinedConstraintParameter (COMMA userDefinedConstraintParameter)* R_BRACE 
;

generalConstraint :  userDefinedConstraint | tableConstraint | contentsConstraint 
;
userDefinedConstraintParameter : 
	governor (COLON 
 		value 
 		| valueSet
 		| object
 		| objectSet
 		)?
;

tableConstraint : /*simpleTableConstraint |*/ componentRelationConstraint 
;
simpleTableConstraint : objectSet 
;


contentsConstraint : 
   CONTAINING_LITERAL type 
 |  ENCODED_LITERAL BY_LITERAL value 
 |  CONTAINING_LITERAL type ENCODED_LITERAL BY_LITERAL value 
;


subtypeConstraint	:	
elementSetSpecs
//((value | MIN_LITERAL) LESS_THAN? DOUBLE_DOT LESS_THAN?  (value | MAX_LITERAL) )
//	| sizeConstraint
//	| value
	;
value  :   bt=builtinValue
-> ^(VALUE_NODE $bt?)
;
builtinValue :  
		enumeratedValue
	|	integerValue
	|	choiceValue
	|	objectIdentifierValue
	|	booleanValue
	|	bitValue
	
 ;
 
objectIdentifierValue : L_BRACE! /*(definedValue)?*/ objIdComponentsList R_BRACE! 
;
objIdComponentsList  
	: 	(objIdComponents) (objIdComponents)*
;
objIdComponents  : 
	    	NUMBER
	|    	IDENTIFIER (L_PARAN (NUMBER | definedValue ) R_PARAN)?
	|    	definedValue 
;
 
 
integerValue :  signedNumber | IDENTIFIER       
;

choiceValue  :    IDENTIFIER COLON value 
;
enumeratedValue  : IDENTIFIER 
;

signedNumber :  (MINUS)? NUMBER
;
choiceType    : CHOICE_LITERAL L_BRACE alternativeTypeLists R_BRACE -> ^(CHOICE_LITERAL alternativeTypeLists)
;
alternativeTypeLists :   rootAlternativeTypeList (COMMA! 
   extensionAndException  extensionAdditionAlternatives  optionalExtensionMarker )? 
	;
extensionAdditionAlternatives  : (COMMA!  extensionAdditionAlternativesList )? 
;
extensionAdditionAlternativesList  : (extensionAdditionAlternative) (COMMA  extensionAdditionAlternative)* 
;
extensionAdditionAlternative  :  extensionAdditionAlternativesGroup | namedType 
;
extensionAdditionAlternativesGroup  :  DOUBLE_L_BRACKET  versionNumber  alternativeTypeList  DOUBLE_R_BRACKET 
;

rootAlternativeTypeList  : alternativeTypeList
;
alternativeTypeList : (namedType) (COMMA! namedType)* 
;
namedType : id=IDENTIFIER   t=type -> ^(NODE_INFO ^($id $t))
;  
enumeratedType : ENUMERATED_LITERAL L_BRACE en=enumerations R_BRACE -> ^(ENUMERATED_LITERAL $en)
;
enumerations :rootEnumeration (COMMA!   ELLIPSIS! exceptionSpec (COMMA!   additionalEnumeration )?)?
	;
rootEnumeration : enumeration
;
enumeration : enumerationItem ( COMMA! enumerationItem)*
;
enumerationItem : IDENTIFIER | namedNumber | value
;
namedNumber :   IDENTIFIER L_PARAN (sn=signedNumber | dv=definedValue) R_PARAN -> ^(IDENTIFIER $sn? $dv?)
;
definedValue : 
 // externalValueReference 
 //| valuereference 
  parameterizedValue 
;
parameterizedValue : simpleDefinedValue (actualParameterList)?
;
simpleDefinedValue : IDENTIFIER (DOT IDENTIFIER)?
;

actualParameterList : L_BRACE ap1=actualParameter (COMMA ap2=actualParameter)* R_BRACE -> ^($ap1 $ap2?)
;
actualParameter : tp=type | vl=value /*| valueSet | definedObjectClass | object | objectSet*/
-> ^(NODE_INFO $tp? $vl?)
;
exceptionSpec : (EXCLAM  exceptionIdentification)? 
;
exceptionIdentification : signedNumber 
 |     definedValue 
 |     type COLON value 
;
additionalEnumeration : enumeration
;
integerType : INTEGER_LITERAL (L_BRACE nnl=namedNumberList R_BRACE)? (L_PARAN SIZE_LITERAL* (c1=constraint | c2=sizeConstraint) R_PARAN)? (DEFAULT_LITERAL signedNumber)?
	    -> ^(INTEGER_LITERAL ^($nnl)? ^(TAG_SPECIFIC $c1? $c2?))
;
namedNumberList : (nn=namedNumber) (COMMA nn1+=namedNumber)* -> ^(TAG_SPECIFIC $nn $nn1*)
;
objectidentifiertype  :  OBJECT_LITERAL IDENTIFIER_LITERAL -> ^(NODE_INFO["OBJECT_IDENTIFIER"])
;	
relativeoidtype  :  RELATIVE_OID_LITERAL -> ^(NODE_INFO["RELATIVE_OID"])
;	


componentRelationConstraint : L_BRACE (IDENTIFIER (DOT IDENTIFIER)?) R_BRACE  
			     (L_BRACE atNotation (COMMA atNotation)* R_BRACE)?
;
atNotation :  (A_ROND | (A_ROND_DOT level)) componentIdList 
;
level : (DOT level)? 
;

componentIdList : IDENTIFIER (DOT IDENTIFIER)*  //?????
;
octetStringType  :  OCTET_LITERAL STRING_LITERAL (L_PARAN sl=SIZE_LITERAL* (c1=constraint | c2=sizeConstraint) R_PARAN)? 
		-> ^(NODE_INFO["OCTET_STRING"] ^(TAG_SPECIFIC $sl? $c1? $c2?))
;

ia5stringType  :  IA5_STRING_LITERAL (L_PARAN sl=SIZE_LITERAL* (c1=constraint | c2=sizeConstraint) R_PARAN)? 
		-> ^(NODE_INFO["IA5String"] ^(TAG_SPECIFIC $sl? $c1? $c2?))
;
printableStringType  :  PRINTABLE_STRING_LITERAL (L_PARAN sl=SIZE_LITERAL* (c1=constraint | c2=sizeConstraint) R_PARAN)?
                -> ^(NODE_INFO["PrintableString"] ^(TAG_SPECIFIC $sl? $c1? $c2?))
;


numericStringType  :  NUMERIC_STRING_LITERAL (L_PARAN sl=SIZE_LITERAL* (c1=constraint | c2=sizeConstraint) R_PARAN)?
                -> ^(NODE_INFO["NumericString"] ^(TAG_SPECIFIC $sl? $c1? $c2?))
;

nullType :	 NULL_LITERAL
;
booleanType : BOOLEAN_LITERAL	
;

bitStringType    : (BIT_LITERAL STRING_LITERAL) (L_BRACE nb=namedBitList R_BRACE)? (DEFAULT_LITERAL L_BRACE nb2=namedBitList R_BRACE)? (L_PARAN sl=SIZE_LITERAL* (c1=constraint | c2=sizeConstraint) R_PARAN)?
		-> ^(NODE_INFO["BIT_STRING"] ^(TAG_SPECIFIC $nb? $sl? $c1? $c2?))
;
namedBitList: (namedBit) (COMMA! namedBit)* 
;
namedBit      : IDENTIFIER L_PARAN? (num=NUMBER | dv=definedValue)? R_PARAN? -> ^(IDENTIFIER $num? $dv?)
	;
	
booleanValue:  TRUE_LITERAL | FALSE_LITERAL | TRUE_SMALL_LITERAL | FALSE_SMALL_LITERAL
;

bitValue: BSTRING
;
	
	
	
	

//EXTENSTIONENDMARKER  :  COMMA  ELLIPSIS 
//;

UNRESTRICTEDCHARACTERSTRINGTYPE : CHARACTER_LITERAL STRING_LITERAL 
;
EXTENSTIONENDMARKER  :  COMMA  ELLIPSIS 
;

fragment
DIGIT	:	'0'..'9'
	;
fragment
UPPER 	: ('A'..'Z');
fragment
LOWER	: ('a'..'z');		

NUMBER	:	DIGIT+;


//WORD : UPPER+;
		
WS  :  (' '|'\r'|'\t'|'\u000C'|'\n') {$channel=HIDDEN;}
    ;


fragment
Exponent : ('e'|'E') ('+'|'-')? NUMBER ;

LINE_COMMENT
	: '--' ~('\n'|'\r')* '\r'? '\n' {$channel=HIDDEN;}
	;


	
BSTRING
: APOSTROPHE ('0'..'1')* '\'B'
;
fragment	
HEXDIGIT : (DIGIT|'a'..'f'|'A'..'F') ;
HSTRING  : APOSTROPHE HEXDIGIT* '\'H' ;

    


//IDENTIFIER : ('a'..'z'|'A'..'Z') ('0'..'9'|'a'..'z'|'A'..'Z')* ;
CSTRING
    :  '"' ( EscapeSequence | ~('\\'|'"') )* '"'
    ;
	
fragment
EscapeSequence
    :   '\\' ('b'|'t'|'n'|'f'|'r'|'\"'|APOSTROPHE|'\\')
    ;



//fragment

/**I found this char range in JavaCC's grammar, but Letter and Digit overlap.
   Still works, but...
 */
fragment
LETTER 
    :  '\u0024' |
    	'\u002d' |
     '\u0041'..'\u005a' |
       '\u005f' |
       '\u0061'..'\u007a' |
       '\u00c0'..'\u00d6' |
       '\u00d8'..'\u00f6' |
       '\u00f8'..'\u00ff' |
       '\u0100'..'\u1fff' |
       '\u3040'..'\u318f' |
       '\u3300'..'\u337f' |
       '\u3400'..'\u3d2d' |
       '\u4e00'..'\u9fff' |
       '\uf900'..'\ufaff'
    ;

fragment
JavaIDDigit 
    :  '\u0030'..'\u0039' |
       '\u0660'..'\u0669' |
       '\u06f0'..'\u06f9' |
       '\u0966'..'\u096f' |
       '\u09e6'..'\u09ef' |
       '\u0a66'..'\u0a6f' |
       '\u0ae6'..'\u0aef' |
       '\u0b66'..'\u0b6f' |
       '\u0be7'..'\u0bef' |
       '\u0c66'..'\u0c6f' |
       '\u0ce6'..'\u0cef' |
       '\u0d66'..'\u0d6f' |
       '\u0e50'..'\u0e59' |
       '\u0ed0'..'\u0ed9' |
       '\u1040'..'\u1049'
   ;

//OBJECTCLASSREFERENCE 
//	: UPPER (UPPER | LOWER | '-')
//	;
IDENTIFIER 
    :   LETTER (LETTER|JavaIDDigit)*
    ;
