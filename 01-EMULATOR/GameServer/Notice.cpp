// Notice.cpp: implementation of the CNotice class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Notice.h"
#include "MemScript.h"
#include "Util.h"

CNotice gNotice;

static int Notice_SplitLines(const char* in, char out[][MAX_MESSAGE_SIZE + 1], int maxLines)
{
	if (in == 0 || maxLines <= 0) return 0;

	int line = 0;
	int j = 0;

	for (int i = 0; in[i] != 0; i++)
	{
		char c = in[i];

		// aceita separador por | e também \n literal e newline real
		if (c == '|')
		{
			out[line][j] = 0;
			line++;
			j = 0;
			if (line >= maxLines) break;
			continue;
		}

		// literal "\n"
		if (c == '\\' && in[i + 1] == 'n')
		{
			out[line][j] = 0;
			line++;
			j = 0;
			i++; // pula o 'n'
			if (line >= maxLines) break;
			continue;
		}

		// newline real
		if (c == '\n' || c == '\r')
		{
			out[line][j] = 0;
			line++;
			j = 0;
			// se for \r\n, pula o \n
			if (c == '\r' && in[i + 1] == '\n') i++;
			if (line >= maxLines) break;
			continue;
		}

		if (j < MAX_MESSAGE_SIZE)
		{
			out[line][j++] = c;
		}
	}

	out[line][j] = 0;
	return (line + 1);
}

 
 // Converte texto com separadores em "multi-string" para o client:
 // Entrada aceita:
 //   - "\\n" (texto literal no arquivo) => nova linha
 //   - "\\r\\n" (texto literal)         => nova linha
 //   - '\n' e '\r\n' (já reais)         => nova linha
 //   - '|'                              => nova linha
 // Saída:
 //   "linha1\0linha2\0linha3\0"
 // Retorna o total de bytes (incluindo o \0 final).
 static int Notice_BuildMultiString(char* text,int maxLen)
 {
 	if(text == 0 || maxLen <= 0){ return 0; }
 
 	char* src = text;
 	char* dst = text;
 	char* end = text + (maxLen-1); // deixa espaço pro \0 final
 
 	while(*src != 0 && dst < end)
 	{
 		// literal "\\r\\n"
 		if(src[0] == '\\' && src[1] == 'r' && src[2] == '\\' && src[3] == 'n')
 		{
 			*dst++ = 0;
 			src += 4;
 			continue;
 		}
 
 		// literal "\\n"
 		if(src[0] == '\\' && src[1] == 'n')
 		{
 			*dst++ = 0;
 			src += 2;
 			continue;
 		}
 
 		// real "\r\n"
 		if(src[0] == '\r' && src[1] == '\n')
 		{
 			*dst++ = 0;
 			src += 2;
 			continue;
 		}
 
 		// real '\n' ou '\r'
 		if(src[0] == '\n' || src[0] == '\r')
 		{
 			*dst++ = 0;
 			src += 1;
 			continue;
 		}
 
 		// '|'
 		if(*src == '|')
 		{
 			*dst++ = 0;
 			src++;
 			continue;
 		}
 
 		*dst++ = *src++;
 	}
 
 	// garante \0 final
 	*dst++ = 0;
 
 	// total bytes
 	return (int)(dst - text);
 }

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNotice::CNotice() // OK
{
	this->m_count = 0;

	this->m_NoticeValue = 0;

	this->m_NoticeTime = GetTickCount();
}

CNotice::~CNotice() // OK
{

}

void CNotice::Load(char* path) // OK
{
	CMemScript* lpMemScript = new CMemScript;

	if(lpMemScript == 0)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR,path);
		return;
	}

	if(lpMemScript->SetBuffer(path) == 0)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
		delete lpMemScript;
		return;
	}

	this->m_count = 0;

	try
	{
		while(true)
		{
			if(lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			if(strcmp("end",lpMemScript->GetString()) == 0)
			{
				break;
			}

			NOTICE_INFO info;

			memset(&info,0,sizeof(info));

			strcpy_s(info.Message,lpMemScript->GetString());

			info.Type = lpMemScript->GetAsNumber();

			info.Count = lpMemScript->GetAsNumber();

			info.Opacity = lpMemScript->GetAsNumber();

			info.Delay = lpMemScript->GetAsNumber();

			info.Color = RGB(lpMemScript->GetAsNumber(),lpMemScript->GetAsNumber(),lpMemScript->GetAsNumber());

			info.Speed = lpMemScript->GetAsNumber();

			info.RepeatTime = lpMemScript->GetAsNumber()*1000;

			this->SetInfo(info);
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

void CNotice::SetInfo(NOTICE_INFO info) // OK
{
	if(this->m_count < 0 || this->m_count >= MAX_NOTICE)
	{
		return;
	}

	this->m_NoticeInfo[this->m_count++] = info;
}

void CNotice::MainProc() // OK
{
	if(this->m_count == 0)
	{
		return;
	}

	NOTICE_INFO* lpInfo = &this->m_NoticeInfo[this->m_NoticeValue];

	if((GetTickCount()-this->m_NoticeTime) >= ((DWORD)lpInfo->RepeatTime))
	{
		this->m_NoticeValue = (((this->m_NoticeValue+1)>=this->m_count)?0:(this->m_NoticeValue+1));
		this->m_NoticeTime = GetTickCount();
		this->GCNoticeSendToAll(lpInfo->Type,lpInfo->Count,lpInfo->Opacity,lpInfo->Delay,lpInfo->Color,lpInfo->Speed,"%s",lpInfo->Message);
	}
}

void CNotice::GCNoticeSend(int aIndex,BYTE type,BYTE count,BYTE opacity,WORD delay,DWORD color,BYTE speed,char* message,...) // OK
{
	char buff[256] = {0};

	va_list arg;
	va_start(arg,message);
	vsprintf_s(buff,message,arg);
	va_end(arg);

	// Type 0 no S3 é 1 linha: se tiver multilinha, envia várias notices separadas
	if (type == 0 && (strchr(buff, '|') != 0 || strstr(buff, "\\n") != 0 || strchr(buff, '\n') != 0))
	{
		char lines[10][MAX_MESSAGE_SIZE + 1] = { 0 };
		int total = Notice_SplitLines(buff, lines, 10);

		for (int n = 0; n < total; n++)
		{
			if (lines[n][0] == 0) continue;
			// manda 1 por vez (count=1)
			this->GCNoticeSend(aIndex, type, 1, opacity, delay, color, speed, "%s", lines[n]);
		}
		return;
	}

	int size = Notice_BuildMultiString(buff,sizeof(buff)); // inclui \0 final
 
 	size = ((size>MAX_MESSAGE_SIZE)?MAX_MESSAGE_SIZE:size);

	PMSG_NOTICE_SEND pMsg;

	pMsg.header.set(0x0D, (sizeof(pMsg) - (sizeof(pMsg.message) - size)));

	pMsg.type = type;

	pMsg.count = count;

	pMsg.opacity = opacity;

	pMsg.delay = delay;

	pMsg.color = color;

	pMsg.speed = speed;

	memcpy(pMsg.message, buff, size);

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void CNotice::GCNoticeSendToAll(BYTE type,BYTE count,BYTE opacity,WORD delay,DWORD color,BYTE speed,char* message,...) // OK
{
	char buff[256] = {0};

	va_list arg;
	va_start(arg,message);
	vsprintf_s(buff,message,arg);
	va_end(arg);

	if (type == 0 && (strchr(buff, '|') != 0 || strstr(buff, "\\n") != 0 || strchr(buff, '\n') != 0))
	{
		char lines[10][MAX_MESSAGE_SIZE + 1] = { 0 };
		int total = Notice_SplitLines(buff, lines, 10);

		for (int n = 0; n < total; n++)
		{
			if (lines[n][0] == 0) continue;
			this->GCNoticeSendToAll(type, 1, opacity, delay, color, speed, "%s", lines[n]);
		}
		return;
	}

	int size = Notice_BuildMultiString(buff,sizeof(buff)); // inclui \0 final
 
 	size = ((size>MAX_MESSAGE_SIZE)?MAX_MESSAGE_SIZE:size);

	PMSG_NOTICE_SEND pMsg;

	// 'size' já inclui o \0 final
 	pMsg.header.set(0x0D,(sizeof(pMsg)-(sizeof(pMsg.message)-size)));

	pMsg.type = type;

	pMsg.count = count;

	pMsg.opacity = opacity;

	pMsg.delay = delay;

	pMsg.color = color;

	pMsg.speed = speed;

	memcpy(pMsg.message, buff, size);

	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnectedGP(n) != 0)
		{
			DataSend(n,(BYTE*)&pMsg,pMsg.header.size);
		}
	}
}