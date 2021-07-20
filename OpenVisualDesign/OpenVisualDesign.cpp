// OpenVisualDesign.cpp : Defines the entry point for the application.
//

#include "OpenVisualDesign.h"
#include "Header.h"
#include "UserInterface/UserInterface.h"
#include <imgui.h>
#include <clang-c/Index.h>
#include <iostream>

using namespace std;

ostream& operator<<(ostream& stream, const CXString& str)
{
	stream << clang_getCString(str);
	clang_disposeString(str);
	return stream;
}

OpenVisualDesign::OpenVisualDesign()
{

	headers.push_back(std::make_unique<OVD::Header>("..\\..\\..\\..\\example.h"));
}

int main()
{
	CXIndex index = clang_createIndex(0, 0);
	CXTranslationUnit unit = clang_parseTranslationUnit(
		index,
		"..\\..\\..\\..\\example.hpp", nullptr, 0,
		nullptr, 0,
		CXTranslationUnit_VisitImplicitAttributes);
	
	CXCursor cursor = clang_getTranslationUnitCursor(unit);

	CXToken* tokens;
	unsigned int nbTokens;
	CXSourceRange srcRange;

	srcRange = clang_getCursorExtent(cursor);

	clang_tokenize(unit, srcRange, &tokens, &nbTokens);

	for (int i = 0; i < nbTokens; ++i)
	{
		CXToken currentToken = tokens[i];

		CXString tokenString = clang_getTokenSpelling(unit, currentToken);
		CXTokenKind kind = clang_getTokenKind(currentToken);

		switch (kind) {
		case CXToken_Comment:
			printf("Token : %s \t| COMMENT\n", clang_getCString(tokenString));
			break;
		case CXToken_Identifier:
			printf("Token : %s \t| IDENTIFIER\n", clang_getCString(tokenString));
			break;
		case CXToken_Keyword:
			printf("Token : %s \t| KEYWORD\n", clang_getCString(tokenString));
			break;
		case CXToken_Literal:
			printf("Token : %s \t| LITERAL\n", clang_getCString(tokenString));
			break;
		case CXToken_Punctuation:
			printf("Token : %s \t| PUNCTUATION\n", clang_getCString(tokenString));
			break;
		default:
			break;
		}
	}

	clang_disposeTokens(unit, tokens, nbTokens);

	clang_visitChildren(
		cursor,
		[](CXCursor c, CXCursor parent, CXClientData client_data)
		{

			//if (clang_getCursorKind(c) == CXCursorKind::CXCursor_AnnotateAttr)
			//if(clang_Cursor_Evaluate(c))
			
			{
				std::cout << "Cursor '" << clang_getCursorSpelling(c) << "' of kind '"
					<< clang_getCursorKindSpelling(clang_getCursorKind(c)) << "'\n";
			}
			return CXChildVisit_Recurse;
		},
		nullptr);
		
	OVD::UserInterface ui;
	ui.run();
	return 0;
}
