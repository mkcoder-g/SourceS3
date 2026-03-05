// QuizEvent.cpp: implementação do Evento de Quiz

#include "stdafx.h"
#include "QuizEvent.h"
#include "GameMain.h"
#include "DSProtocol.h"
#include "Log.h"
#include "MemScript.h"
#include "Notice.h"
#include "ItemManager.h"
#include "MuCashShop.h"
#include "Protocol.h"
#include "Util.h"

#include <algorithm>
#include <cctype>
#include <cstdarg>
#include <ctime>
#include <cstring>
#include <random>

CQuizEvent gQuizEvent;

// ------------------------------------------------------------------
// Helpers locais
// ------------------------------------------------------------------
static void TrimString(std::string& s)
{
    auto notSpace = [](int ch){ return (ch!=' ' && ch!='\t' && ch!='\r' && ch!='\n'); };
    while(!s.empty() && !notSpace((unsigned char)s.front())) s.erase(s.begin());
    while(!s.empty() && !notSpace((unsigned char)s.back())) s.pop_back();
}

// overload para buffers char[] (legacy)
static void TrimString(char* s, size_t maxLen)
{
    if (s == nullptr || maxLen == 0)
        return;
    std::string tmp(s);
    TrimString(tmp);
    #if defined(_MSC_VER)
    strncpy_s(s, maxLen, tmp.c_str(), _TRUNCATE);
    #else
    std::strncpy(s, tmp.c_str(), maxLen - 1);
    s[maxLen - 1] = 0;
    #endif
}

static std::mt19937& QuizRng()
{
    static std::mt19937 rng((unsigned int)time(0) ^ (unsigned int)GetTickCount());
    return rng;
}

CQuizEvent::CQuizEvent()
{
    // OBS: por padrão fica ATIVADO. Se quiser desligar, use QuizEventSwitch=0 no Custom.dat
    this->m_Switch = 1;
    this->m_IntervalMinutes = 30;
    this->m_AnswerTimeSeconds = 30;
    this->m_AnnounceNoWinner = 1;
    this->m_AutoStart = 1;
    this->m_FirstStartMinutes = 5;
    this->m_AcceptAnswerInChat = 1;
    this->m_NoRepeat = 1;
    this->m_RankingSwitch = 1;
    this->m_SpamDelaySeconds = 2;

    this->m_IpSpamSwitch = 1;
    this->m_IpSpamDelaySeconds = 2;
    this->m_MaxTriesPerQuestion = 3;
    this->m_AccentInsensitive = 1;

    // Recompensas (padrão: 0)
    this->m_RewardZen = 0;
    this->m_RewardCoin1 = 0; // compat
    this->m_RewardPCPoints = 0;
    this->m_RewardBless = 0;
    this->m_RewardSoul = 0;
    this->m_RewardChaos = 0;
    this->m_RewardCreation = 0;

    // Item (padrão: desativado)
    this->m_RewardItemType = -1;
    this->m_RewardItemIndex = -1;
    this->m_RewardItemLevel = 0;
    this->m_RewardItemDur = 0;
    this->m_RewardItemOpt = 0;
    this->m_RewardItemExc = 0;
    this->m_RewardItemSet = 0;
    this->m_RewardItemCount = 0;

    this->m_State = 0;
    this->m_ServerStartTick = GetTickCount();
    this->m_NextStartTick = 0;
    this->m_QuestionStartTick = 0;
    this->m_CurrentQuestionPos = -1;

    this->m_Order.clear();
    this->m_OrderPos = 0;
    this->m_Hits.clear();
    this->m_LastTryTick.clear();
    this->m_LastTryTickIp.clear();
    this->m_TryCountByIndex.clear();
    this->m_TryCountByIp.clear();
    this->m_Questions.clear();
}

CQuizEvent::~CQuizEvent()
{
}

void CQuizEvent::ReadQuizEventInfo(char* section, char* path)
{
    // Config no GameServerInfo - Custom.dat (opcional)
    this->m_Switch = GetPrivateProfileInt(section, "QuizEventSwitch", this->m_Switch, path);
    this->m_IntervalMinutes = GetPrivateProfileInt(section, "QuizEventIntervalMinutes", this->m_IntervalMinutes, path);
    this->m_AnswerTimeSeconds = GetPrivateProfileInt(section, "QuizEventAnswerTimeSeconds", this->m_AnswerTimeSeconds, path);
    this->m_AnnounceNoWinner = GetPrivateProfileInt(section, "QuizEventAnnounceNoWinner", this->m_AnnounceNoWinner, path);
    this->m_AutoStart = GetPrivateProfileInt(section, "QuizEventAutoStart", this->m_AutoStart, path);
    this->m_FirstStartMinutes = GetPrivateProfileInt(section, "QuizEventFirstStartMinutes", this->m_FirstStartMinutes, path);
    this->m_AcceptAnswerInChat = GetPrivateProfileInt(section, "QuizEventAcceptAnswerInChat", this->m_AcceptAnswerInChat, path);
    this->m_NoRepeat = GetPrivateProfileInt(section, "QuizEventNoRepeat", this->m_NoRepeat, path);
    this->m_RankingSwitch = GetPrivateProfileInt(section, "QuizEventRankingSwitch", this->m_RankingSwitch, path);
    this->m_SpamDelaySeconds = GetPrivateProfileInt(section, "QuizEventSpamDelaySeconds", this->m_SpamDelaySeconds, path);

    this->m_IpSpamSwitch = GetPrivateProfileInt(section, "QuizEventIpSpamSwitch", this->m_IpSpamSwitch, path);
    this->m_IpSpamDelaySeconds = GetPrivateProfileInt(section, "QuizEventIpSpamDelaySeconds", this->m_IpSpamDelaySeconds, path);
    this->m_MaxTriesPerQuestion = GetPrivateProfileInt(section, "QuizEventMaxTriesPerQuestion", this->m_MaxTriesPerQuestion, path);
    this->m_AccentInsensitive = GetPrivateProfileInt(section, "QuizEventAccentInsensitive", this->m_AccentInsensitive, path);

    this->m_RewardZen = GetPrivateProfileInt(section, "QuizEventRewardZen", this->m_RewardZen, path);
    this->m_RewardCoin1 = 0; // unused (use only QuizEventRewardPCPoints)
    this->m_RewardPCPoints = GetPrivateProfileInt(section, "QuizEventRewardPCPoints", this->m_RewardPCPoints, path);
    this->m_RewardBless = GetPrivateProfileInt(section, "QuizEventRewardBless", this->m_RewardBless, path);
    this->m_RewardSoul = GetPrivateProfileInt(section, "QuizEventRewardSoul", this->m_RewardSoul, path);
    this->m_RewardChaos = GetPrivateProfileInt(section, "QuizEventRewardChaos", this->m_RewardChaos, path);
    this->m_RewardCreation = GetPrivateProfileInt(section, "QuizEventRewardCreation", this->m_RewardCreation, path);

    this->m_RewardItemType = GetPrivateProfileInt(section, "QuizEventRewardItemType", this->m_RewardItemType, path);
    this->m_RewardItemIndex = GetPrivateProfileInt(section, "QuizEventRewardItemIndex", this->m_RewardItemIndex, path);
    this->m_RewardItemLevel = GetPrivateProfileInt(section, "QuizEventRewardItemLevel", this->m_RewardItemLevel, path);
    this->m_RewardItemDur = GetPrivateProfileInt(section, "QuizEventRewardItemDur", this->m_RewardItemDur, path);
    this->m_RewardItemOpt = GetPrivateProfileInt(section, "QuizEventRewardItemOpt", this->m_RewardItemOpt, path);
    this->m_RewardItemExc = GetPrivateProfileInt(section, "QuizEventRewardItemExc", this->m_RewardItemExc, path);
    this->m_RewardItemSet = GetPrivateProfileInt(section, "QuizEventRewardItemSet", this->m_RewardItemSet, path);
    this->m_RewardItemCount = GetPrivateProfileInt(section, "QuizEventRewardItemCount", this->m_RewardItemCount, path);
}

void CQuizEvent::Load(char* path)
{
    CMemScript* lpMemScript = new CMemScript;

    if(lpMemScript == 0)
    {
        ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR, path);
        return;
    }

    if(lpMemScript->SetBuffer(path) == 0)
    {
        ErrorMessageBox(lpMemScript->GetLastError());
        delete lpMemScript;
        return;
    }

    this->m_Questions.clear();

    try
    {
        while(true)
        {
            if(lpMemScript->GetToken() == TOKEN_END)
            {
                break;
            }

            if(strcmp("end", lpMemScript->GetString()) == 0)
            {
                break;
            }

            QUIZ_QUESTION_INFO info;
            info.Index = lpMemScript->GetNumber();
            info.Question = lpMemScript->GetAsString();
            info.Answer = lpMemScript->GetAsString();

            this->m_Questions.push_back(info);
        }
    }
    catch(...)
    {
        ErrorMessageBox(lpMemScript->GetLastError());
    }

    delete lpMemScript;

    this->ResetOrder();
    gLog.Output(LOG_GENERAL, "[QuizEvent] Loaded %d questions", (int)this->m_Questions.size());
}

void CQuizEvent::MainProc()
{
    if(this->m_Switch == 0)
    {
        return;
    }

    DWORD tick = GetTickCount();

    if(this->m_State == 0)
    {
        if(this->m_Questions.empty() != 0)
        {
            return;
        }

        // auto start
        if(this->m_AutoStart == 0)
        {
            return;
        }

        if(this->m_NextStartTick == 0)
        {
            // primeiro start pode ser diferente do intervalo
            DWORD first = (DWORD)((this->m_FirstStartMinutes < 0 ? 0 : this->m_FirstStartMinutes) * 60 * 1000);
            if(first > 0)
            {
                this->m_NextStartTick = (this->m_ServerStartTick > 0 ? this->m_ServerStartTick : tick);
                if((tick - this->m_NextStartTick) >= first)
                {
                    this->StartNewQuestion(false);
                }
                return;
            }
            this->m_NextStartTick = tick;
        }

        if((tick - this->m_NextStartTick) >= (DWORD)(this->m_IntervalMinutes * 60 * 1000))
        {
            this->StartNewQuestion(false);
        }
        return;
    }

    // Question active
    if(this->m_State == 1)
    {
        if(this->m_AnswerTimeSeconds > 0 && (tick - this->m_QuestionStartTick) >= (DWORD)(this->m_AnswerTimeSeconds * 1000))
        {
            this->FinishQuestionNoWinner();
        }
    }
}

void CQuizEvent::CommandAnswer(LPOBJ lpObj, char* arg)
{
    if(this->m_Switch == 0)
    {
        return;
    }

    if(this->m_State != 1 || this->m_CurrentQuestionPos < 0 || this->m_CurrentQuestionPos >= (int)this->m_Questions.size())
    {
        this->SendPlayerMessage(lpObj->Index, "[QUIZ] Nao existe quiz ativo agora.");
        return;
    }

    char answer[256] = {0};
    strncpy_s(answer, arg, sizeof(answer)-1);
    TrimString(answer, sizeof(answer));

    if(strcmp(answer, "") == 0)
    {
        this->SendPlayerMessage(lpObj->Index, "Use: /quiz <resposta>");
        return;
    }

    // Anti-spam tambem para /quiz
    if(this->CheckAntiSpam(lpObj) == 0)
    {
        return;
    }

    if(this->CheckAnswer(answer) == 0)
    {
        // feedback só pro player
        this->SendPlayerMessage(lpObj->Index, "[QUIZ] Resposta incorreta!");
        return;
    }

    this->FinishQuestionWithWinner(lpObj);
}

bool CQuizEvent::IsSpecialChatPrefix(char c)
{
    return (c == '/' || c == '!' || c == '~' || c == '@');
}

bool CQuizEvent::TryAnswerFromChat(LPOBJ lpObj, const char* message)
{
    if(this->m_Switch == 0 || this->m_AcceptAnswerInChat == 0)
    {
        return 0;
    }

    if(message == 0 || message[0] == 0 || IsSpecialChatPrefix(message[0]) != 0)
    {
        return 0;
    }

    if(this->m_State != 1 || this->m_CurrentQuestionPos < 0 || this->m_CurrentQuestionPos >= (int)this->m_Questions.size())
    {
        return 0;
    }

    char answer[256] = {0};
    strncpy_s(answer, message, sizeof(answer)-1);
    TrimString(answer, sizeof(answer));

    if(strcmp(answer, "") == 0)
    {
        return 0; // nao consome mensagem vazia
    }

    // Anti-spam: nao bloqueia o chat, apenas impede validacao frequente
    if(this->CheckAntiSpam(lpObj) == 0)
    {
        return 0;
    }

    if(this->CheckAnswer(answer) == 0)
    {
        // NAO consome resposta errada: deixa aparecer no chat.
        this->SendPlayerMessage(lpObj->Index, "[QUIZ] Resposta incorreta!");
        return 0;
    }

    this->FinishQuestionWithWinner(lpObj);
    return 1; // consome apenas quando acertou (evita vazar a resposta no chat)
}

void CQuizEvent::CommandRank(LPOBJ lpObj)
{
    this->SendRankToPlayer(lpObj);
}

void CQuizEvent::CommandStart(LPOBJ lpObj)
{
    if(this->m_Switch == 0)
    {
        this->SendPlayerMessage(lpObj->Index, "[QUIZ] Evento desativado (QuizEventSwitch=0).");
        return;
    }

    this->StartNewQuestion(true);

    gLog.Output(LOG_COMMAND, "[QuizEvent][%s][%s] - GM Start", lpObj->Account, lpObj->Name);
}

void CQuizEvent::CommandStop(LPOBJ lpObj)
{
    if(this->m_State == 0)
    {
        this->SendPlayerMessage(lpObj->Index, "[QUIZ] Nao existe quiz ativo.");
        return;
    }

    this->m_State = 0;
    this->m_CurrentQuestionPos = -1;
    this->m_QuestionStartTick = 0;
    this->m_NextStartTick = GetTickCount();

    this->SendGlobalQuizMessage("[QUIZ] O quiz foi encerrado pelo GM.");
    gLog.Output(LOG_COMMAND, "[QuizEvent][%s][%s] - GM Stop", lpObj->Account, lpObj->Name);
}

void CQuizEvent::CommandNext(LPOBJ lpObj)
{
    if(this->m_Switch == 0)
    {
        return;
    }

    this->StartNewQuestion(true);
    gLog.Output(LOG_COMMAND, "[QuizEvent][%s][%s] - GM Next", lpObj->Account, lpObj->Name);
}

void CQuizEvent::StartNewQuestion(bool forced)
{
    if(this->m_Questions.empty() != 0)
    {
        return;
    }

    this->m_CurrentQuestionPos = this->GetRandomQuestionIndex();
    this->m_State = 1;
    this->m_QuestionStartTick = GetTickCount();
    this->m_NextStartTick = this->m_QuestionStartTick; // base do próximo intervalo (será resetado ao finalizar)

    // libera tentativas/contadores no inicio de cada pergunta
    this->m_LastTryTick.clear();
    this->m_LastTryTickIp.clear();
    this->m_TryCountByIndex.clear();
    this->m_TryCountByIp.clear();

    const QUIZ_QUESTION_INFO& q = this->m_Questions[this->m_CurrentQuestionPos];

    if(forced != 0)
    {
        this->SendGlobalQuizMessage("[QUIZ] (GM) Nova pergunta: %s", q.Question.c_str());
    }
    else
    {
        this->SendGlobalQuizMessage("[QUIZ] Pergunta: %s", q.Question.c_str());
    }

    if(this->m_AcceptAnswerInChat != 0)
    {
        this->SendGlobalQuizMessage("Responda no chat (ou /quiz <resposta>) - Tempo: %d segundos", this->m_AnswerTimeSeconds);
    }
    else
    {
        this->SendGlobalQuizMessage("/quiz <resposta> (Tempo: %d segundos)", this->m_AnswerTimeSeconds);
    }
}

void CQuizEvent::FinishQuestionNoWinner()
{
    if(this->m_CurrentQuestionPos < 0 || this->m_CurrentQuestionPos >= (int)this->m_Questions.size())
    {
        this->m_State = 0;
        this->m_CurrentQuestionPos = -1;
        this->m_QuestionStartTick = 0;
        this->m_NextStartTick = GetTickCount();
        return;
    }

    if(this->m_AnnounceNoWinner != 0)
    {
        const QUIZ_QUESTION_INFO& q = this->m_Questions[this->m_CurrentQuestionPos];
        this->SendGlobalQuizMessage("[QUIZ] Tempo esgotado! Resposta correta: %s", q.Answer.c_str());
    }

    this->m_State = 0;
    this->m_CurrentQuestionPos = -1;
    this->m_QuestionStartTick = 0;
    this->m_NextStartTick = GetTickCount();
}

void CQuizEvent::FinishQuestionWithWinner(LPOBJ lpObj)
{
    if(this->m_CurrentQuestionPos < 0 || this->m_CurrentQuestionPos >= (int)this->m_Questions.size())
    {
        return;
    }

    this->GiveReward(lpObj);
    this->AddHit(lpObj);

    const QUIZ_QUESTION_INFO& q = this->m_Questions[this->m_CurrentQuestionPos];
    this->SendGlobalQuizMessage("[QUIZ] Vencedor: %s! Resposta: %s", lpObj->Name, q.Answer.c_str());

    this->m_State = 0;
    this->m_CurrentQuestionPos = -1;
    this->m_QuestionStartTick = 0;
    this->m_NextStartTick = GetTickCount();
}

bool CQuizEvent::CheckAnswer(const char* playerAnswer) const
{
    if(this->m_CurrentQuestionPos < 0 || this->m_CurrentQuestionPos >= (int)this->m_Questions.size())
    {
        return 0;
    }

    std::string a1 = Normalize(playerAnswer);
    std::string answers = this->m_Questions[this->m_CurrentQuestionPos].Answer;

    // suporte a varias respostas validas: "brazil|brasil|br"
    // OBS: cada token passa por Normalize antes de comparar
    if(answers.empty())
    {
        return 0;
    }

    std::string token;
    for(size_t i=0;i<=answers.size();i++)
    {
        char c = (i<answers.size()? answers[i] : '|');
        if(c == '|')
        {
            std::string a2 = Normalize(token.c_str());
            if(!a2.empty() && a1 == a2)
            {
                return 1;
            }
            token.clear();
        }
        else
        {
            token.push_back(c);
        }
    }

    return 0;
}

bool CQuizEvent::CheckAntiSpam(LPOBJ lpObj)
{
    if(lpObj == 0)
    {
        return 0;
    }

    DWORD now = GetTickCount();

    // Delay por player (segundos)
    if(this->m_SpamDelaySeconds > 0)
    {
        std::string key(lpObj->Name);
        auto it = this->m_LastTryTick.find(key);
        if(it != this->m_LastTryTick.end())
        {
            DWORD diff = (now - it->second);
            DWORD need = (DWORD)(this->m_SpamDelaySeconds * 1000);
            if(diff < need)
            {
                DWORD left = (need - diff) / 1000;
                if(left <= 0) left = 1;
                this->SendPlayerMessage(lpObj->Index, "[QUIZ] Aguarde %d segundos para tentar novamente.", (int)left);
                return 0;
            }
        }
        this->m_LastTryTick[key] = now;
    }

    // Delay por IP (anti multi-client)
    std::string ip = "0.0.0.0";
    if(lpObj->IpAddr[0] != 0)
    {
        ip = std::string(lpObj->IpAddr);
    }

    if(this->m_IpSpamSwitch != 0 && this->m_IpSpamDelaySeconds > 0)
    {
        auto it2 = this->m_LastTryTickIp.find(ip);
        if(it2 != this->m_LastTryTickIp.end())
        {
            DWORD diff = (now - it2->second);
            DWORD need = (DWORD)(this->m_IpSpamDelaySeconds * 1000);
            if(diff < need)
            {
                DWORD left = (need - diff) / 1000;
                if(left <= 0) left = 1;
                this->SendPlayerMessage(lpObj->Index, "[QUIZ] Aguarde %d segundos (anti-spam IP).", (int)left);
                return 0;
            }
        }
        this->m_LastTryTickIp[ip] = now;
    }

    // Limite de tentativas por pergunta (0 = ilimitado)
    if(this->m_MaxTriesPerQuestion > 0)
    {
        int& cnt = this->m_TryCountByIndex[lpObj->Index];
        cnt++;

        if(cnt > this->m_MaxTriesPerQuestion)
        {
            this->SendPlayerMessage(lpObj->Index, "[QUIZ] Limite de %d tentativas nesta pergunta.", this->m_MaxTriesPerQuestion);
            return 0;
        }

        // reforço por IP contra multi-client
        if(this->m_IpSpamSwitch != 0)
        {
            int& ipcnt = this->m_TryCountByIp[ip];
            ipcnt++;

            // IP tem um limite um pouco maior pra não punir party/lan house
            int ipMax = (this->m_MaxTriesPerQuestion * 2);
            if(ipMax < this->m_MaxTriesPerQuestion) ipMax = this->m_MaxTriesPerQuestion;

            if(ipcnt > ipMax)
            {
                this->SendPlayerMessage(lpObj->Index, "[QUIZ] Limite de tentativas por IP atingido nesta pergunta.");
                return 0;
            }
        }
    }

    return 1;
}


std::string CQuizEvent::Normalize(const char* text)
{
    std::string s = (text == 0 ? "" : text);

    // trim
    while(!s.empty() && (s.front() == ' ' || s.front() == '\t' || s.front() == '\r' || s.front() == '\n'))
    {
        s.erase(s.begin());
    }
    while(!s.empty() && (s.back() == ' ' || s.back() == '\t' || s.back() == '\r' || s.back() == '\n'))
    {
        s.pop_back();
    }

    // tolower (ASCII)
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return (char)std::tolower(c); });

    // Aceitar com/sem acento:
    // - CP1252/Latin-1 (muito comum em Season 3)
    // - e UTF-8 para os caracteres mais usados em PT-BR
    auto replace_all = [](std::string& str, const char* from, const char* to)
    {
        size_t pos = 0;
        size_t fromLen = strlen(from);
        size_t toLen = strlen(to);
        while((pos = str.find(from, pos)) != std::string::npos)
        {
            str.replace(pos, fromLen, to);
            pos += toLen;
        }
    };

    // UTF-8 (sequências)
    replace_all(s, "\xC3\xA1", "a"); // á
    replace_all(s, "\xC3\xA0", "a"); // à
    replace_all(s, "\xC3\xA2", "a"); // â
    replace_all(s, "\xC3\xA3", "a"); // ã
    replace_all(s, "\xC3\xA4", "a"); // ä
    replace_all(s, "\xC3\xA9", "e"); // é
    replace_all(s, "\xC3\xAA", "e"); // ê
    replace_all(s, "\xC3\xA8", "e"); // è
    replace_all(s, "\xC3\xAB", "e"); // ë
    replace_all(s, "\xC3\xAD", "i"); // í
    replace_all(s, "\xC3\xAC", "i"); // ì
    replace_all(s, "\xC3\xAE", "i"); // î
    replace_all(s, "\xC3\xAF", "i"); // ï
    replace_all(s, "\xC3\xB3", "o"); // ó
    replace_all(s, "\xC3\xB2", "o"); // ò
    replace_all(s, "\xC3\xB4", "o"); // ô
    replace_all(s, "\xC3\xB5", "o"); // õ
    replace_all(s, "\xC3\xB6", "o"); // ö
    replace_all(s, "\xC3\xBA", "u"); // ú
    replace_all(s, "\xC3\xB9", "u"); // ù
    replace_all(s, "\xC3\xBB", "u"); // û
    replace_all(s, "\xC3\xBC", "u"); // ü
    replace_all(s, "\xC3\xA7", "c"); // ç

    // CP1252/Latin-1 (byte a byte)
    for(size_t i=0;i<s.size();i++)
    {
        unsigned char ch = (unsigned char)s[i];
        switch(ch)
        {
            // a
            case 0xE1: case 0xE0: case 0xE2: case 0xE3: case 0xE4: s[i] = 'a'; break;
            // e
            case 0xE9: case 0xEA: case 0xE8: case 0xEB: s[i] = 'e'; break;
            // i
            case 0xED: case 0xEC: case 0xEE: case 0xEF: s[i] = 'i'; break;
            // o
            case 0xF3: case 0xF2: case 0xF4: case 0xF5: case 0xF6: s[i] = 'o'; break;
            // u
            case 0xFA: case 0xF9: case 0xFB: case 0xFC: s[i] = 'u'; break;
            // c
            case 0xE7: s[i] = 'c'; break;
            default: break;
        }
    }

    return s;
}


void CQuizEvent::SendGlobalQuizMessage(const char* fmt, ...)
{
    char buff[256] = {0};

    va_list ap;
    va_start(ap, fmt);
    vsnprintf_s(buff, sizeof(buff), _TRUNCATE, fmt, ap);
    va_end(ap);

    // Global NOTICE (na tela) para todos.
    // Type 0 = notice padro; Count 1.
    gNotice.GCNoticeSendToAll(0,1,0,0,0,0,buff);
}

void CQuizEvent::SendPlayerMessage(int aIndex, const char* fmt, ...)
{
    char buff[256] = {0};

    va_list ap;
    va_start(ap, fmt);
    vsnprintf_s(buff, sizeof(buff), _TRUNCATE, fmt, ap);
    va_end(ap);

    gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,buff);
}

void CQuizEvent::GiveReward(LPOBJ lpObj)
{
    // Zen
    if(this->m_RewardZen > 0)
    {
        if(gObjCheckMaxMoney(lpObj->Index, (DWORD)this->m_RewardZen) != 0)
        {
            lpObj->Money += (DWORD)this->m_RewardZen;
            GCMoneySend(lpObj->Index, lpObj->Money);
        }
    }

    // Coin1 / PC Points
    int totalPC = (this->m_RewardPCPoints);
    if(totalPC > 0)
    {
        // Alguns sources S3 dependem do DataServer (MuCashShop) para persistir Coin1.
        // Aqui garantimos duas coisas:
        // 1) Tentamos persistir via DataServer (se existir/conectado)
        // 2) Atualizamos imediatamente no client (e no lpObj) para funcionar mesmo sem DS.
        gObjCoinAdd(lpObj->Index, totalPC);

        lpObj->Coin1 = (DWORD)(lpObj->Coin1 + totalPC);
        PMSG_MU_CASH_SHOP_POINT_SEND pMsg;
        pMsg.header.set(0xF5,0x04,sizeof(pMsg));
        pMsg.Point = lpObj->Coin1;
        DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);
    }

    // Jewels / Items: create in inventory if there is space.
    // If no space, drop on ground near the player.

    auto HasInventorySpaceFor = [&](int itemIndex)->bool
    {
        ITEM_INFO ii;
        if(gItemManager.GetInfo(itemIndex,&ii) == 0)
        {
            return false;
        }

        int MaxY = (gItemManager.GetInventoryMaxValue(lpObj)-INVENTORY_WEAR_SIZE)/8;

        for(int y=0;y<MaxY;y++)
        {
            for(int x=0;x<8;x++)
            {
                if(lpObj->InventoryMap[((y*8)+x)] == 0xFF)
                {
                    if(gItemManager.InventoryRectCheck(lpObj->Index,x,y,ii.Width,ii.Height) != 0xFF)
                    {
                        return true;
                    }
                }
            }
        }

        return false;
    };

    auto CreateOneToInventoryOrDrop = [&](int itemIndex, BYTE level, BYTE dur, BYTE opt1, BYTE opt2, BYTE opt3, BYTE exc, BYTE set)
    {
        BYTE ItemSocketOption[MAX_SOCKET_OPTION] = {0};

        if(HasInventorySpaceFor(itemIndex) != 0)
        {
            // 0xEB = cria direto no inventario (padrao do SourceS3: CommandManager/PersonalShop/CustomJewelPack)
            GDCreateItemSend(lpObj->Index,0xEB,0,0,itemIndex,level,dur,opt1,opt2,opt3,-1,exc,set,0,0,ItemSocketOption,0xFF,0);
            return;
        }

        // Sem espaco -> drop no chao
        BYTE px = (BYTE)lpObj->X;
        BYTE py = (BYTE)lpObj->Y;
        GDCreateItemSend(lpObj->Index,lpObj->Map,px,py,itemIndex,level,dur,opt1,opt2,opt3,lpObj->Index,exc,set,0,0,ItemSocketOption,0xFF,0);
        this->SendPlayerMessage(lpObj->Index, "[QUIZ] Inventario cheio: premio dropado no chao.");
    };

    auto GiveMany = [&](int itemIndex, BYTE level, BYTE dur, BYTE opt1, BYTE opt2, BYTE opt3, BYTE exc, BYTE set, int count)
    {
        if(count <= 0) return;
        for(int i=0;i<count;i++)
        {
            CreateOneToInventoryOrDrop(itemIndex,level,dur,opt1,opt2,opt3,exc,set);
        }
    };

    // Jewels
    auto GiveJewel = [&](int itemType, int itemIndex, int count)
    {
        if(count <= 0) return;
        int item = GET_ITEM(itemType, itemIndex);
        GiveMany(item, 0, 0, 0, 0, 0, 0, 0, count);
    };
    GiveJewel(14,13, this->m_RewardBless);
    GiveJewel(14,14, this->m_RewardSoul);
    GiveJewel(12,15, this->m_RewardChaos);
    GiveJewel(14,22, this->m_RewardCreation);

    // Item
    if(this->m_RewardItemType >= 0 && this->m_RewardItemIndex >= 0 && this->m_RewardItemCount > 0)
    {
        int itemIndex = GET_ITEM(this->m_RewardItemType, this->m_RewardItemIndex);
        // opt/luck/skill: Option3 = RewardItemOpt
        GiveMany(
            itemIndex,
            (BYTE)this->m_RewardItemLevel,
            (BYTE)this->m_RewardItemDur,
            0,0,(BYTE)this->m_RewardItemOpt,
            (BYTE)this->m_RewardItemExc,
            (BYTE)this->m_RewardItemSet,
            this->m_RewardItemCount);
    }


    gLog.Output(LOG_GENERAL, "[QuizEvent][%s][%s] Reward Zen:%d PC:%d Bless:%d Soul:%d Chaos:%d Creation:%d Item:(%d,%d) x%d", lpObj->Account, lpObj->Name, this->m_RewardZen, (this->m_RewardPCPoints), this->m_RewardBless, this->m_RewardSoul, this->m_RewardChaos, this->m_RewardCreation, this->m_RewardItemType, this->m_RewardItemIndex, this->m_RewardItemCount);
}

void CQuizEvent::AddHit(LPOBJ lpObj)
{
    if(this->m_RankingSwitch == 0 || lpObj == 0)
    {
        return;
    }
    this->m_Hits[std::string(lpObj->Name)]++;
}

void CQuizEvent::SendRankToPlayer(LPOBJ lpObj)
{
    if(lpObj == 0)
    {
        return;
    }

    if(this->m_RankingSwitch == 0)
    {
        this->SendPlayerMessage(lpObj->Index, "[QUIZ] Ranking desativado.");
        return;
    }

    if(this->m_Hits.empty())
    {
        this->SendPlayerMessage(lpObj->Index, "[QUIZ] Ranking vazio ainda.");
        return;
    }

    // copia e ordena
    std::vector<std::pair<std::string,int>> v;
    v.reserve(this->m_Hits.size());
    for(auto it = this->m_Hits.begin(); it != this->m_Hits.end(); ++it)
    {
        v.push_back(*it);
    }

    std::sort(v.begin(), v.end(), [](const auto& a, const auto& b){
        if(a.second != b.second) return a.second > b.second;
        return a.first < b.first;
    });

    this->SendPlayerMessage(lpObj->Index, "[QUIZ] ===== RANKING (Top 10) =====");
    int max = (v.size() > 10 ? 10 : (int)v.size());
    for(int i=0;i<max;i++)
    {
        this->SendPlayerMessage(lpObj->Index, "[QUIZ] %d) %s - %d", (i+1), v[i].first.c_str(), v[i].second);
    }
}

void CQuizEvent::ResetOrder()
{
    this->m_Order.clear();
    this->m_OrderPos = 0;

    int count = (int)this->m_Questions.size();
    this->m_Order.reserve(count);
    for(int i=0;i<count;i++)
    {
        this->m_Order.push_back(i);
    }

    if(count > 1)
    {
        std::shuffle(this->m_Order.begin(), this->m_Order.end(), QuizRng());
    }
}

void CQuizEvent::EnsureOrder()
{
    if(this->m_NoRepeat == 0)
    {
        return;
    }

    if(this->m_Order.empty() || this->m_OrderPos >= (int)this->m_Order.size())
    {
        this->ResetOrder();
    }
}

int CQuizEvent::GetRandomQuestionIndex()
{
    int count = (int)this->m_Questions.size();
    if(count <= 0)
    {
        return -1;
    }

    if(this->m_NoRepeat != 0)
    {
        this->EnsureOrder();
        int pos = this->m_Order[this->m_OrderPos++];
        return pos;
    }

    return (rand() % count);
}
