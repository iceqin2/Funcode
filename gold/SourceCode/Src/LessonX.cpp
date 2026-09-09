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
	m_iGameState			=	0;
	m_pGameBegin = new CSprite("GameBegin");//实验二修改
	m_iGetGoldState = 0;
	m_fHookRotation = 0.f;
	m_iHookRotToLeft = 1;
	m_fHookStartPosX = 0.f;
	m_fHookStartPosY = 0.f;
	m_pGoldenHook = new CSprite("GoldHook");
	m_pGoldenMan = new CAnimateSprite("GoldMan");
//实验三修改
	m_iGoldCount = 0;
	m_iGoldBornMinX = 0;
	m_iGoldBornMaxX = 0;
	m_iGoldBornMinY = 0;
	m_iGoldBornMaxY = 0;
	m_pGoldenTemplate = new CSprite("GoldTemplate");
//实验四修改
	m_fEmptyHookSpeed = 15.f;//实验五修改
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
	// 画钩子的缆绳线。不管游戏是什么状态，这根缆绳线都要画出来
	DrawHookLine();//实验五修改

}
//=============================================================================
//
// 每局开始前进行初始化，清空上一局相关数据
void CGameMain::GameInit()
{
	m_iGoldCount = 20;//实验四修改
	// 重设钩子从0度、由右往左自动旋转
	m_fHookRotation = 0.f;
	m_iHookRotToLeft = 1;
	// 初始化抓取状态
	m_iGetGoldState = 0;
	// 以下变量只需要初始化一次。
	m_iInitedHookPos = 0;
	if (0 == m_iInitedHookPos)
	{	//钩子可以发出碰撞
		m_pGoldenHook->SetSpriteCollisionSend(true);
		//金子可以接收碰撞
		m_pGoldenTemplate->SetSpriteCollisionReceive(true);
	//实验五
		m_iGoldBornMinX = CSystem::GetScreenLeft() + 5;
		m_iGoldBornMaxX = CSystem::GetScreenRight() - 5;
		m_iGoldBornMinY = CSystem::GetScreenTop() + 20;
		m_iGoldBornMaxY = CSystem::GetScreenBottom() - 5;//实验四修改

		m_iInitedHookPos = 1;
		// 钩子初始位置值初始化
		m_fHookStartPosX = m_pGoldenHook->GetSpritePositionX();
		m_fHookStartPosY = m_pGoldenHook->GetSpritePositionY();
		//钩子的世界边界初始化
		m_pGoldenHook->SetSpriteWorldLimit(WORLD_LIMIT_NULL, CSystem::GetScreenLeft(), CSystem::GetScreenTop(), CSystem::GetScreenRight(), CSystem::GetScreenBottom());
	
		int		iLoop = 0;
		const char* szName = new char[15];
		int		iSize = 4, iPosX = 0, iPosY = 0;
		for (iLoop = 0; iLoop < m_iGoldCount; iLoop++)
		{
			if (iLoop < 10)
			{
				iSize = 4;
			}
			else if (iLoop >= 10 && iLoop < 16)
			{
				iSize = 6;
			}
			else
			{
				iSize = 8;
			}
			// 初始化金子精灵实例
			char* tmpBlock;
			tmpBlock = CSystem::MakeSpriteName("GoldBlock", iLoop);
			CSprite* tmpGlodenBlock = new CSprite(tmpBlock);
			tmpGlodenBlock->CloneSprite("GoldTemplate");
			tmpGlodenBlock->SetSpriteWidth((float)iSize);
			tmpGlodenBlock->SetSpriteHeight((float)iSize);


			// 随机一个位置
			iPosX = CSystem::RandomRange(m_iGoldBornMinX, m_iGoldBornMaxX);
			iPosY = CSystem::RandomRange(m_iGoldBornMinY, m_iGoldBornMaxY);
			tmpGlodenBlock->SetSpritePosition((float)iPosX, (float)iPosY);
			m_vGoldenBlock.push_back(*tmpGlodenBlock);
		}
		// 播放挖金者的动作(恢复初始守候动作)
		m_pGoldenMan->AnimateSpritePlayAnimation("GolderManAnimation2", 0);

		// 隐藏游戏开始的提示
		m_pGameBegin->SetSpriteVisible(0);

//实验四修改
	}
	// 播放挖金者的动作(恢复初始守候动作)
	m_pGoldenMan->AnimateSpritePlayAnimation("GolderManAnimation2", 0);

//实验三修改
}
//=============================================================================
//
// 每局游戏进行中
void CGameMain::GameRun(float fDeltaTime)
{// 如果当前为等待抓取状态，那么，左右摇摆钩子
	if (0 == m_iGetGoldState)
	{
		const	float	fRotateSpeed = 45.f; // 摇摆速度，单位度/秒
		float	 fThisRotate = fRotateSpeed * fDeltaTime; // 本次旋转的度数
		if (m_iHookRotToLeft)
		{
			m_fHookRotation += fThisRotate;
			if (m_fHookRotation >= 180.f)
			{
				m_fHookRotation = 180.f;
				m_iHookRotToLeft = 0;
			}
		}
		else
		{
			m_fHookRotation -= fThisRotate;
			if (m_fHookRotation <= 0.f)
			{
				m_fHookRotation = 0.f;
				m_iHookRotToLeft = 1;
			}
		}
		m_pGoldenHook->SetSpriteRotation(m_fHookRotation);
	}
	//实验三修改
			// 如果当前为钩子往回归位，则判断是否已经运动到终点。到了终点，则又开始摇摆、等待
	else if (2 == m_iGetGoldState || 3 == m_iGetGoldState)
	{
		// 判断是否移动到终点(判断的依据是XY方向的移动速度为0，即金子是否已经停止移动)
		float	fSpeedX = m_pGoldenHook->GetSpriteLinearVelocityX();
		float	fSpeedY = m_pGoldenHook->GetSpriteLinearVelocityY();
		// 当前速度不为0，还在运动中
		// 浮点数是否为0，不能直接判断 == != 0
		if (fSpeedX > 0.00001f || fSpeedX < -0.00001f || fSpeedY > 0.00001f || fSpeedY < -0.00001f)
			return;
		// 速度为0，钩子回到初始点，开始下一轮的抓取
	// 当前抓取到金子，将抓取到的金子释放并删除之。然后将金子数量减一
		if (3 == m_iGetGoldState)
		{
			m_iGoldCount--;
			GetCurGoldenSprite(m_szCurGetGold).SpriteDismount();
			GetCurGoldenSprite(m_szCurGetGold).DeleteSprite();
		}
		//实验五修改
		m_iGetGoldState = 0;
		m_pGoldenMan->AnimateSpritePlayAnimation("GolderManAnimation2", 0);//实验五修改

	}
}
//=============================================================================
//
// 本局游戏结束
void CGameMain::GameEnd()
{
	m_pGameBegin->SetSpriteVisible(1);
}
void CGameMain::OnKeyDown(const int iKey, const int iAltPress, const int iShiftPress, const int iCtrlPress)
{
	if (KEY_SPACE == iKey && 0 == m_iGameState)
	{
		m_iGameState = 1;
		m_pGameBegin->SetSpriteVisible(0);
	}// 当前处于游戏进行中，按下向下的方向键，钩子伸出抓取金子
	else if (KEY_DOWN == iKey && 2 == m_iGameState && 0 == m_iGetGoldState)
	{
		// 设置抓取状态为：钩子往外伸
		m_iGetGoldState = 1;
		// 以当前朝向给钩子一个向前的速度
		m_pGoldenHook->SetSpriteLinearVelocityPolar(m_fEmptyHookSpeed, m_fHookRotation);
		// 播放挖金者的动作(一个胳膊往下压的动作)
		m_pGoldenMan->AnimateSpritePlayAnimation("GolderManAnimation1", 0);
	}
//实验五修改

}
//实验二添加
void CGameMain::DrawHookLine()
{// 首先，从矿工精灵上获取一个缆绳链接点作为绳子的起始点(该链接点在编辑器里编辑好)
	float	fStartX = m_pGoldenMan->GetSpriteLinkPointPosX(1);
	float	fStartY = m_pGoldenMan->GetSpriteLinkPointPosY(1);
	// 绳子终点在钩子精灵上获取(该链接点在编辑器里编辑好)
	float	fEndX = m_pGoldenHook->GetSpriteLinkPointPosX(1);
	float	fEndY = m_pGoldenHook->GetSpriteLinkPointPosY(1);
	// 在这两点之间划线.线的颜色红绿蓝值都为50，即灰色
	CSystem::DrawLine(fStartX, fStartY, fEndX, fEndY, 2.f, 0, 50, 50, 50, 255);

}
//实验五修改
void CGameMain::OnSpriteColSprite(const char szSrcName[], const char szTarName[])
{
	if (2 != m_iGameState || 1 != m_iGetGoldState)
		return;
	if (stricmp("GoldHook", szSrcName) != 0 && stricmp("GoldHook", szTarName) != 0)
		return;
	// 找到哪个是金子的名字
	const char* szGoldName = NULL;
	if (strstr(szSrcName, "GoldBlock"))
		szGoldName = szSrcName;
	else if (strstr(szTarName, "GoldBlock"))
		szGoldName = szTarName;
	else
		return;
	GetCurGoldenSprite(szGoldName).SpriteMountToSpriteLinkPoint("GoldHook", 2);//实验五修改
	// 设置抓取状态为：钩子往回收，抓取到东西。记录金子名字
	m_iGetGoldState = 3;
	strcpy(m_szCurGetGold, szGoldName);
	float	fWidth;
	fWidth = GetCurGoldenSprite(szGoldName).GetSpriteWidth();
	float	fSpeed = ((10.f - fWidth) / 10.f) * m_fEmptyHookSpeed;
	// 钩子往初始位置移动
	m_pGoldenHook->SpriteMoveTo(m_fHookStartPosX, m_fHookStartPosY, fSpeed, 1);
	// 播放挖金者的动作(胳膊来回动的动作)
	m_pGoldenMan->AnimateSpritePlayAnimation("GolderManAnimation3", 0);

//实验五修改

}
//实验五修改
CSprite CGameMain::GetCurGoldenSprite(const char* goldenname)
{
	for (int i = 0; i < m_vGoldenBlock.size(); i++)
		if (strcmp(goldenname, m_vGoldenBlock[i].GetName()) == 0)
			return m_vGoldenBlock[i];
	return NULL;
}
//实验五修改
void CGameMain::OnSpriteColWorldLimit(const char szName[], const int iColSide)
{// 判断是否是钩子碰到边界，往回收
	if (stricmp(szName, "GoldHook") == 0)
	{
		// 设置抓取状态为：钩子往回收，未抓取到东西
		m_iGetGoldState = 2;
		// 钩子往初始位置移动
		m_pGoldenHook->SpriteMoveTo(m_fHookStartPosX, m_fHookStartPosY, m_fEmptyHookSpeed, 1);
		// 播放挖金者的动作(胳膊来回动的动作)
		m_pGoldenMan->AnimateSpritePlayAnimation("GolderManAnimation3", 0);
	}

}
//实验六修改
