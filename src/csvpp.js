function csvpp_trim_whitespace(i_sString)
{
	var sResult = new String();
	var sResultForward = new String();
	var bWhitespace = true;
	for (i = i_sString.length - 1; i >= 0; i--)
	{
		var chI = i_sString.charAt(i);
		if (!bWhitespace || chI != ' ' && chI != '\r' && chI != '\f')
		{
			sResult += chI;
			bWhitespace = false;
		}
	}
	bWhitespace = true;
	for (i = i_sString.length - 1; i >= 0; i--)
	{
		var chI = sResult.charAt(i);
		if (!bWhitespace || chI != ' ' && chI != '\r' && chI != '\f')
		{
			sResultForward += chI;
			bWhitespace = false;
		}
	}
	return sResultForward;
}

class CSV
{
	constructor()
	{ 
		this._eError = "none"; 
		this._vData = new Array();
	}

	parse_csv(i_sData, i_sTokens, i_sStringDelim, i_bTrim_Whitespace, i_bPreserve_String_Delim)
	{
		var nLine = 0;
		var nDataCursor = 0;
		this._vData = new Array();
		if (i_sData !== null && i_sData.length != 0 && i_sTokens !== null && i_sTokens.length != 0 && i_sStringDelim !== null && i_sStringDelim.length != 0)
		{
			var sCurr = new String();
			nLine++;
			var chCurr = i_sData.charAt(nDataCursor);
			nDataCursor++;
			var chLast = 0;
			var vLine = new Array();
			var sElement = new String();
			while (nDataCursor < i_sData.length && this._eError == "none")
			{
				// bypass whitespace
				if (chCurr == '\f' || chCurr == '\r' || chCurr == '\n')
				{
					if (chLast != '\f' && chLast != '\r' && chLast != '\n')
					{
						if (i_bTrim_Whitespace)
							sElement = csvpp_trim_whitespace(sElement);
						vLine.push(sElement);
						this._vData.push(vLine);
						sElement = new String();
						vLine = new Array();
						nLine++;
					}
				}
				else if (nDataCursor < i_sData.length)
				{
					
					var pTokenCursor = 0;
					var bIsToken = false;
					while (!bIsToken && pTokenCursor < i_sTokens.length)
					{
						bIsToken = chCurr == i_sTokens.charAt(pTokenCursor);
						pTokenCursor++;
					}
					if (bIsToken)
					{
						if (i_bTrim_Whitespace)
							sElement = csvpp_trim_whitespace(sElement);
						vLine.push(sElement);
						sElement = new String();
					}
					else
					{
						var pStringCursor = 0;
						var bIsStringStart = false;
						var nStringDelim = -1;
						while (!bIsStringStart && pStringCursor < i_sStringDelim.length)
						{
							nStringDelim++;
							bIsStringStart = chCurr == i_sStringDelim.charAt(pStringCursor);
							pStringCursor++;
						}
						if (bIsStringStart)
						{
							if (i_bPreserve_String_Delim)
							{
								sElement += chCurr;
							}
							chLast = chCurr;
							chCurr = i_sData.charAt(nDataCursor);
							nDataCursor++;
							var bIsStringDone = chCurr == i_sStringDelim.charAt(nStringDelim);
							while (!bIsStringDone && nDataCursor < i_sData.length && chCurr != '\f' && chCurr != '\r' && chCurr != '\n')
							{
								sElement += chCurr;
								chLast = chCurr;
								chCurr = i_sData.charAt(nDataCursor);
								nDataCursor++;
								bIsStringDone = chCurr == i_sStringDelim[nStringDelim];
							}
							if (bIsStringDone && i_bPreserve_String_Delim)
							{
								sElement += chCurr;
							}
							if (!bIsStringDone)
							{
								if (nDataCursor < i_sData.length)
									this._eError == "unterminated string eof";
								else
									this._eError == "unterminated string eol";
							}
						}
						else
						{
							sElement += chCurr;
						}
					}
				}
				if (nDataCursor < i_sData.length && this._eError == "none")
				{
					chLast = chCurr;
					chCurr = i_sData.charAt(nDataCursor);
					nDataCursor++;
				}
			}
			if (chLast != '\f' && chLast != '\r' && chLast != '\n' && this._eError == "none")
			{
				if (i_bTrim_Whitespace)
					sElement = csvpp_trim_whitespace(sElement);
				vLine.push(sElement);
				this._vData.push(vLine);
			}
		}
	}
	at(row, col)
	{
		return this._vData[row][col];
	}
}

