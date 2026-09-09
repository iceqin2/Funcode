/////////////////////////////////////////////////////////////////////////////////
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////
#include <Stdio.h>
#include "CommonClass.h"
#include "LessonX.h"
////////////////////////////////////////////////////////////////////////////////
//
//
CGameMain		g_GameMain;	

//==============================================================================
//
// 大体的程序流程为：GameMainLoop函数为主循环函数，在引擎每帧刷新屏幕图像之后，都会被调用一次。

//==============================================================================
//
// 构造函数
CGameMain::CGameMain()
{
	m_pMyFish = new CSprite("myfish");//实验六修改
	m_pFish = new CSprite("fish0");//实验二修改
	m_iGameState			=	1;//实验三修改
	m_iGameState = 1;
	m_fScreenBottom = 0.f;
	m_fScreenLeft = 0.f;
	m_fScreenRight = 0.f;
	m_fScreenTop = 0.f;//实验三修改
	m_fSpeedBottom = 0.f;
	m_fSpeedLeft = 0.f;
	m_fSpeedRight = 0.f;
	m_fSpeedTop = 0.f;
//实验七修改

}
//==============================================================================
//
// 析构函数
CGameMain::~CGameMain()
{
}

//==============================================================================
//
// 游戏主循环，此函数将被不停的调用，引擎每刷新一次屏幕，此函数即被调用一次
// 用以处理游戏的开始、进行中、结束等各种状态. 
// 函数参数fDeltaTime : 上次调用本函数到此次调用本函数的时间间隔，单位：秒
void CGameMain::GameMainLoop( float	fDeltaTime )
{
	switch( GetGameState() )
	{
		// 初始化游戏，清空上一局相关数据
	case 1:
		{
			GameInit();
			SetGameState(2); // 初始化之后，将游戏状态设置为进行中
		}
		break;

		// 游戏进行中，处理各种游戏逻辑
	case 2:
		{
			// TODO 修改此处游戏循环条件，完成正确游戏逻辑
			if( true )
			{
				GameRun( fDeltaTime );
			}
			else // 游戏结束。调用游戏结算函数，并把游戏状态修改为结束状态
			{				
				SetGameState(0);
				GameEnd();
			}
		}
		break;

		// 游戏结束/等待按空格键开始
	case 0:
	default:
		break;
	};
}
//=============================================================================
//
// 每局开始前进行初始化，清空上一局相关数据
CSprite* CGameMain::FindSpriteByName(const char* szName)
{
	for (int i = 0; i < m_vFish.size(); i++)
		if (strcmp(szName, m_vFish[i]->GetName()) == 0)
			return m_vFish[i];

}//实验五添加

void CGameMain::GameInit()
{	
	// 生成多条鱼精灵
	for (int i = 0; i < 4; i++)
	{
		char  szName[128];
		float fPosX, fPosY;
		float fSpeedX;
		//将szName的值赋为“fish”加上i，即循环因子
		sprintf(szName, "fish%d", i);
		CSprite* tmpSprite = new CSprite(szName);
		tmpSprite->CloneSprite("fish_muban");	//创建鱼精灵
		fPosX = CSystem::RandomRange(m_fScreenLeft + 10.f, m_fScreenRight - 10.f);
		fPosY = CSystem::RandomRange(m_fScreenTop + 10.f, m_fScreenBottom - 10.f);
		tmpSprite->SetSpritePosition(fPosX, fPosY);
		tmpSprite->SetSpriteWorldLimit(WORLD_LIMIT_NULL, m_fScreenLeft - 20.f, m_fScreenTop, m_fScreenRight + 20.f, m_fScreenBottom);
		fSpeedX = CSystem::RandomRange(10, 20);
		tmpSprite->SetSpriteLinearVelocity(fSpeedX, 0);
		m_vFish.push_back(tmpSprite);

	}//实验五修改

	// 获取屏幕的边界值
	m_fScreenLeft = CSystem::GetScreenLeft();
	m_fScreenRight = CSystem::GetScreenRight();
	m_fScreenTop = CSystem::GetScreenTop();
	m_fScreenBottom = CSystem::GetScreenBottom();

	// 设置精灵世界边界
	m_pFish->SetSpriteWorldLimit(WORLD_LIMIT_NULL, m_fScreenLeft, m_fScreenTop, m_fScreenRight, m_fScreenBottom);

	m_pFish->SetSpriteLinearVelocity(15, 0);
//实验三修改
}
//=============================================================================
//
// 每局游戏进行中
void CGameMain::GameRun( float fDeltaTime )
{
	m_pFish->SetSpriteLinearVelocity(15, 0);//实验二修改
}
//=============================================================================
//
// 本局游戏结束
void CGameMain::GameEnd()

{
}
void CGameMain::OnSpriteColWorldLimit(const char* szName, const int iColSide)
{
	/*if (strcmp(szName, "fish0") == 0)
	{
		m_pFish->SetSpriteLinearVelocity(0, 0);
		//dSetSpriteFlipX("fish0", true);
	}*///实验三修改
	bool	bFlip=false;
	float	fSpeedX=0.0f;//豆包修改
	float fPosY = 0.0f;//豆包修改
	if (strcmp(szName, "fish0") == 0)
	{
		if (iColSide == 1)  // 右边
		{
			bFlip = true;
			fSpeedX = -CSystem::RandomRange(10, 20);
		}
		else if (iColSide == 0)   // 左边
		{
			bFlip = false;
			fSpeedX = CSystem::RandomRange(10, 20);
		}
		CSprite* tmpSprite = FindSpriteByName(szName);
		tmpSprite->SetSpriteFlipX(bFlip);
		tmpSprite->SetSpriteLinearVelocity(fSpeedX, 0);							fPosY = CSystem::RandomRange(m_fScreenTop + 10.f,
			m_fScreenBottom - 10.f);
		//因为有不同的鱼，因此随机生成Y方向的速度后需要分配给不同的
		//鱼，这样多条鱼就不会重复了
		tmpSprite->SetSpritePositionY(fPosY);//实验五修改


	}
//实验四修改
	
}
void CGameMain::OnKeyDown(const int iKey, const int iAltPress, const int iShiftPress, const int iCtrlPress)
{
	switch (iKey)
	{
	case KEY_W:
		m_fSpeedTop = -10.f;
		break;
	case KEY_A:
		m_fSpeedLeft = -15.f;
		break;
	case KEY_S:
		m_fSpeedBottom = 10.f;
		break;
	case KEY_D:
		m_fSpeedRight = 15.f;
		break;
	}Swimming();
	/*if ((m_fSpeedLeft + m_fSpeedRight) > 0)
		m_pMyFish->SetSpriteFlipX(false);
	else if ((m_fSpeedLeft + m_fSpeedRight) < 0)
		m_pMyFish->SetSpriteFlipX(true);
	m_pMyFish->SetSpriteLinearVelocity(m_fSpeedLeft + m_fSpeedRight, m_fSpeedTop + m_fSpeedBottom);*/
	/*float	fPosX = 0.f, fPosY = 0.f;
	switch (iKey)
	{
	case KEY_W:
		fPosY = -10.f; //向上的时候Y方向速度为负的
		break;
	case KEY_A:
		fPosX = -15.f;	//向左的时候X方向的速度为负值
		break;
	case KEY_S:
		fPosY = 10.f; //向下的时候Y方向的速度为正值
		break;
	case KEY_D:
		fPosX = 15.f;//向右的时候X方向的速度为正值
		break;
	}
	m_pMyFish->SetSpriteLinearVelocity(fPosX, fPosY);*/
}
//实验六修改
void CGameMain::OnKeyUp(const int iKey)
{
	switch (iKey)
	{
	case KEY_W:
		m_fSpeedTop = 0.f;
		break;
	case KEY_A:
		m_fSpeedLeft = 0.f;
		break;
	case KEY_S:
		m_fSpeedBottom = 0.f;
		break;
	case KEY_D:
		m_fSpeedRight = 0.f;
		break;

	}
	Swimming();
	/*if ((m_fSpeedLeft + m_fSpeedRight) > 0)
		m_pMyFish->SetSpriteFlipX(false);
	else if ((m_fSpeedLeft + m_fSpeedRight) < 0)
		m_pMyFish->SetSpriteFlipX(true);

	m_pMyFish->SetSpriteLinearVelocity(m_fSpeedLeft + m_fSpeedRight, m_fSpeedTop + m_fSpeedBottom);*/
	/*float	fSpeedX = 0.0f, fSpeedY = 0.0f;
	switch (iKey)
	{
	case KEY_W:
	case KEY_A:
	case KEY_S:
	case KEY_D:
		fSpeedX = 0.f;
		fSpeedY = 0.f;
	}
	m_pMyFish->SetSpriteLinearVelocity(fSpeedX, fSpeedY);*/
}//实验六修改
void CGameMain::Swimming()
{
	if ((m_fSpeedLeft + m_fSpeedRight) > 0)
		m_pMyFish->SetSpriteFlipX(false);
	else if ((m_fSpeedLeft + m_fSpeedRight) < 0)
		m_pMyFish->SetSpriteFlipX(true);

	m_pMyFish->SetSpriteLinearVelocity(m_fSpeedLeft + m_fSpeedRight, m_fSpeedTop + m_fSpeedBottom);

}


