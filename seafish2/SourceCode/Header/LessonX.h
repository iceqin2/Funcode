/////////////////////////////////////////////////////////////////////////////////
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////
#ifndef _LESSON_X_H_
#define _LESSON_X_H_
//
#include <Windows.h>
#include <vector>
using namespace std;

/////////////////////////////////////////////////////////////////////////////////
//
// 游戏总管类。负责处理游戏主循环、游戏初始化、结束等工作
class	CGameMain
{
private:
	CSprite* m_pMyFish;//实验六修改
	int				m_iGameState;				// 游戏状态，0：结束或者等待开始；1：初始化；2：游戏进行中
	vector<CSprite*>	m_vFish;//实验五修改
	CSprite* m_pFish;//实验二修改
	CSprite* FindSpriteByName(const char* szName);//实验五修改
	float		m_fScreenLeft;	
	float		m_fScreenRight;	
	float		m_fScreenTop;	
	float		m_fScreenBottom;	// 实验三修改
	float		m_fSpeedLeft;// 鱼4个方向的速度
	float		m_fSpeedRight;
	float		m_fSpeedTop;
	float		m_fSpeedBottom;
//实验七修改


public:
	CGameMain();            //构造函数
	~CGameMain();           //析构函数  

	// Get方法
	int				GetGameState()											{ return m_iGameState; }
	
	// Set方法
	void			SetGameState( const int iState )				{ m_iGameState	=	iState; }
	
	// 游戏主循环等
	void			GameMainLoop( float	fDeltaTime );
	void			GameInit();
	void			GameRun( float fDeltaTime );
	void			GameEnd();
	void			OnSpriteColWorldLimit(const char* szName, const int iColSide);//实验三修改
	void			OnKeyDown(const int iKey, const int iAltPress, const int iShiftPress, const int iCtrlPress);//实验六修改
	void			OnKeyUp(const int iKey);//实验六修改
	void			Swimming();//实验八修改
};


/////////////////////////////////////////////////////////////////////////////////
// 
extern CGameMain	g_GameMain;

#endif // _LESSON_X_H_