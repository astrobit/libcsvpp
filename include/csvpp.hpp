#pragma once
#include <vector>
#include <string>

namespace csvpp
{
	class csv
	{
	public:
		enum class errortype {none,could_not_open_file,unterminated_string_eof,unterminated_string_eol};
		std::vector<std::vector<std::string> > 	_vData;
		size_t 									_NumColumns;
		errortype								_eError;
		size_t 									_nError_Line;
		
		csv(void)
		{
			_vData.clear();
			_NumColumns = 0;
			_eError = errortype::none;
			_nError_Line = 0;
		}
		void parse_csv(const char * i_sFilename,const char * i_sTokens, const char * i_sStringDelim, bool i_bIgnore_Whitespace, bool i_bPreserve_String_Delim);
		
	};
	
	extern "C" csv parse_csv(const char * i_sFilename,const char * i_sTokens, const char * i_sStringDelim, bool i_bIgnore_Whitespace, bool i_bPreserve_String_Delim);
}


