#include<csvpp.hpp>

void csvpp_trim_whitespace(std::string & i_sString)
{
	std::string sResult;
	bool bWhitespace = true;
	for (auto iterI = i_sString.rbegin(); iterI != i_sString.rend(); iterI++)
	{
		if (!bWhitespace || *iterI != ' ' && *iterI != '\n')
		{
			sResult.push_back(*iterI);
			bWhitespace = false;
		}
	}
	bWhitespace = true;
	i_sString.clear();
	for (auto iterI = sResult.rbegin(); iterI != sResult.rend(); iterI++)
	{
		if (!bWhitespace || *iterI != ' ' && *iterI != '\n')
		{
			i_sString.push_back(*iterI);
			bWhitespace = false;
		}
	}	
}


void csvpp::csv::parse_csv(const char * i_sFilename,const char * i_sTokens, const char * i_sStringDelim, bool i_bIgnore_Whitespace, bool i_bPreserve_String_Delim)
{
	size_t nLine = 0;
	if (i_sTokens != nullptr && i_sTokens[0] != 0 && i_sStringDelim != nullptr && i_sStringDelim[0] != 0)
	{
		std::string sCurr;
		FILE * infile = fopen(i_sFilename,"rt");
		if (infile != nullptr)
		{
			nLine++;
			char chCurr = fgetc(infile);
			char chLast = 0;
			std::vector<std::string> vLine;
			std::string sElement;
			while (chCurr != EOF && _eError == csv::errortype::none)
			{
				// bypass whitespace
				if (chCurr == 0xa || chCurr == 0xd)
				{
					if (chLast != 0xa && chLast != 0xd)
					{
						if (i_bIgnore_Whitespace)
							csvpp_trim_whitespace(sElement);
						vLine.push_back(sElement);
						_vData.push_back(vLine);
						sElement.clear();
						vLine.clear();
						nLine++;
					}
				}
				else if (chCurr != EOF)
				{
					const char * pTokenCursor = i_sTokens;
					bool bIsToken = false;
					while (!bIsToken && pTokenCursor[0] != 0 && pTokenCursor != nullptr)
					{
						bIsToken = chCurr == pTokenCursor[0];
						pTokenCursor++;
					}
					if (bIsToken)
					{
						if (i_bIgnore_Whitespace)
							csvpp_trim_whitespace(sElement);
						vLine.push_back(sElement);
						sElement.clear();
					}
					else
					{
						const char * pStringCursor = i_sStringDelim;
						bool bIsStringStart = false;
						size_t nStringDelim = -1;
						while (!bIsStringStart && pStringCursor[0] != 0 && pStringCursor != nullptr)
						{
							nStringDelim++;
							bIsStringStart = chCurr == pStringCursor[0];
							pStringCursor++;
						}
						if (bIsStringStart)
						{
							if (i_bPreserve_String_Delim)
							{
								sElement.push_back(chCurr);
							}
							chLast = chCurr;
							chCurr = fgetc(infile);
							bool bIsStringDone = chCurr == i_sStringDelim[nStringDelim];
							while (!bIsStringDone && chCurr != EOF && chCurr != 0xa && chCurr != 0xd)
							{
								sElement.push_back(chCurr);
								chLast = chCurr;
								chCurr = fgetc(infile);
								bIsStringDone = chCurr == i_sStringDelim[nStringDelim];
							}
							if (bIsStringDone && i_bPreserve_String_Delim)
							{
								sElement.push_back(chCurr);
							}
							if (!bIsStringDone)
							{
								if (chCurr == EOF)
									_eError == csv::errortype::unterminated_string_eof;
								else
									_eError == csv::errortype::unterminated_string_eol;
							}
						}
						else
						{
							sElement.push_back(chCurr);
						}
					}
				}
				if (chCurr != EOF && _eError == csv::errortype::none)
				{
					chLast = chCurr;
					chCurr = fgetc(infile);
				}
			}
			if (chLast != 0x0a && chLast != 0x0d && _eError == csv::errortype::none)
			{
				if (i_bIgnore_Whitespace)
					csvpp_trim_whitespace(sElement);
				vLine.push_back(sElement);
				_vData.push_back(vLine);
				sElement.clear();
				vLine.clear();
			}
			fclose(infile);
		}
		else
			_eError == csv::errortype::could_not_open_file;
	}
}


csv csvpp::parse_csv(const char * i_sFilename,const char * i_sTokens, const char * i_sStringDelim, bool i_bIgnore_Whitespace, bool i_bPreserve_String_Delim)
{
	return (csv().parse_csv(i_sFilename,i_sTokens, i_sStringDelim, i_bIgnore_Whitespace, i_bPreserve_String_Delim));
}

