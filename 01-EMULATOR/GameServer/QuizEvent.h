// QuizEvent.h: Evento de Quiz (perguntas e respostas pelo chat)
//
// Season 3 - SourceS3
// - Dispara perguntas em intervalo configurável
// - Primeiro jogador que responder corretamente ganha recompensa
// - Resposta via comando: /quiz <resposta>
// - Comandos GM: /quizstart | /quizstop | /quiznext

#pragma once

#include "User.h"
#include <vector>
#include <string>
#include <map>

struct QUIZ_QUESTION_INFO
{
	int Index;
	std::string Question;
	std::string Answer;
};

class CQuizEvent
{
public:
	CQuizEvent();
	virtual ~CQuizEvent();

	void ReadQuizEventInfo(char* section, char* path);
	void Load(char* path);
	void MainProc();

	// Player
	void CommandAnswer(LPOBJ lpObj, char* arg);
	bool TryAnswerFromChat(LPOBJ lpObj, const char* message); // responder sem comando
	void CommandRank(LPOBJ lpObj); // /quizrank

	// GM
	void CommandStart(LPOBJ lpObj);
	void CommandStop(LPOBJ lpObj);
	void CommandNext(LPOBJ lpObj);

private:
	void StartNewQuestion(bool forced);
	void FinishQuestionNoWinner();
	void FinishQuestionWithWinner(LPOBJ lpObj);
	bool CheckAnswer(const char* playerAnswer) const;
	static std::string Normalize(const char* text);
	void SendGlobalQuizMessage(const char* fmt, ...);
	void SendPlayerMessage(int aIndex, const char* fmt, ...);
	void GiveReward(LPOBJ lpObj);
	int GetRandomQuestionIndex();
	void EnsureOrder();
	void ResetOrder();
	void AddHit(LPOBJ lpObj);
	void SendRankToPlayer(LPOBJ lpObj);
	static bool IsSpecialChatPrefix(char c);
	bool CheckAntiSpam(LPOBJ lpObj);


private:
	int m_Switch;
	int m_IntervalMinutes;
	int m_AnswerTimeSeconds;
	int m_AnnounceNoWinner;
	int m_AutoStart;
	int m_FirstStartMinutes;
	int m_AcceptAnswerInChat;
	int m_NoRepeat;
	int m_RankingSwitch;
	int m_SpamDelaySeconds; // anti-spam: tempo minimo entre tentativas por player

	int m_IpSpamSwitch; // anti-spam por IP (anti multi-client)
	int m_IpSpamDelaySeconds; // tempo minimo entre tentativas por IP
	int m_MaxTriesPerQuestion; // limite de tentativas por pergunta (0 = ilimitado)
	int m_AccentInsensitive; // aceitar acento/sem acento

	int m_RewardZen;
	int m_RewardCoin1;
	int m_RewardPCPoints;
	int m_RewardBless;
	int m_RewardSoul;
	int m_RewardChaos;
	int m_RewardCreation;

	// Item reward (opcional)
	int m_RewardItemType;
	int m_RewardItemIndex;
	int m_RewardItemLevel;
	int m_RewardItemDur;
	int m_RewardItemOpt;
	int m_RewardItemExc;
	int m_RewardItemSet;
	int m_RewardItemCount;

	// Estado
	int m_State; // 0=idle, 1=question_active
	DWORD m_NextStartTick;
	DWORD m_QuestionStartTick;
	DWORD m_ServerStartTick;
	int m_CurrentQuestionPos;

	// controle de não repetir
	std::vector<int> m_Order;
	int m_OrderPos;

	// ranking (nome -> acertos)
	std::map<std::string,int> m_Hits;

	// anti-spam (nome -> ultimo tick de tentativa)
	std::map<std::string,DWORD> m_LastTryTick;

	// anti-spam por IP
	std::map<std::string,DWORD> m_LastTryTickIp;

	// limite de tentativas por pergunta
	std::map<int,int> m_TryCountByIndex; // aIndex -> tentativas nesta pergunta
	std::map<std::string,int> m_TryCountByIp; // ip -> tentativas nesta pergunta

	std::vector<QUIZ_QUESTION_INFO> m_Questions;
};

extern CQuizEvent gQuizEvent;
