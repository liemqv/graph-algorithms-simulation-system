/*
	VIEN DAI HOC MO HA NOI
	KHOA CONG NGHE TIN HOC
	----- BAI TAP LON ------
	+ Mon: Lap trinh C tren Windows
	+ Sinh vien: Quang Van Liem
	+ Lop: 08B6
	+ De tai: Xay dung chuong trinh MFC mo ta cac giai thuat tren do thi
*/

#include <afxwin.h>
#include <afxext.h>
#include "resource.h"
#include <math.h>
#include <string.h>


class ung_dung:public CWinApp
{
public:
	BOOL InitInstance();
};

ung_dung chuong_trinh;

#define vocung -1

typedef struct
{
    int a;
    int b;
}dt;

//------------cac bien cuc bo---------------
int sodinh;//so dinh cua do thi
int s; //sinh xuat phat
int t; //dinh dich
int ID_DIALOG; //xac dinh dialog nao xe duoc bat len
int trongso;
BOOL chon_sai=FALSE;


//------------------------------------------

class cua_so:public CFrameWnd
{
private:
	CPoint *dinh;//mang luu vi tri cua cac dinh
	int **e;//ma tran trong so
	
	int *daxet;//mang kiem tra da xet
	int *truoc;//mang kiem tra dinh truoc no
	int *mangtg;
	dt  *u;//mang kieu dt
	
	int i, vtmin;
	int dangxet;// dinh duoc co dinh hien tai
	CStdioFile f;
	//--------------------
	CDC m_memDC;
	CBitmap m_bmp;
	CBrush m_bkbrush;
	int maxX,maxY,hinhve;
	CToolBar tool;//toolbar
	CBitmap bit;
	CDC *ngu_canh,cdctg;
	char str[50];
	//--------dung de ve
	int kc;//mep trai anh
	CPen p;//but
	CBrush b;//mau to
	CFont font;
	int yghichu;//toa do cua so ghi chu
	int xghichu;
	int xchitiet;//toa do cua so bang mo ta chi tiet
	int ychitiet;
	//-------------
	BOOL paint; 
	BOOL menu_tao; 
	BOOL them_dinh; 
	BOOL them_cung; 
	int imenu_tao;
	BOOL play;  // bien kiem tra co dang chay hay ko
	
	int thuat_toan;  //bien de nhan biet thuat toan dang duoc chon
	int do_thi;  //bien de nhan biet do thi nao dang duoc chon
	BOOL bmenu; //kiem tra dang chon nut menu nao
	int  imenu; //kiem tra nut nao dang duoc chon
	int delay; //bien la so giay thuc hien mot buoc cua giai thuat
	BOOL hoan_thanh;//kiem tra thuat toan da hoan thanh hay chua
	BOOL bchon_dothi;
	BOOL bchon_thuat_toan;
	BOOL nhan;
	int dinh_nhan;//de biet dinh nao dang duoc nhan
	int so_dinh_cu;
	HINSTANCE h1;
public:
	cua_so();
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//cac ham thong diep de ve
	afx_msg void OnLButtonDown(UINT nGlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nGlags, CPoint point);
	afx_msg void OnMouseMove(UINT nGlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnTimer( UINT nIDEvent );
	afx_msg void OnClose();
	//----------------------
	void OnThongtin();
	//----------------------
	void ve_lai_nut();
	void ve_dinh(CPoint vt, COLORREF mn, COLORREF md, COLORREF mc, char *hien);
	void ve_cung(int truoc, int sau, COLORREF md, COLORREF mc);
	void ve_do_thi();
	void ve_bang();
	void clrscr_main();
	void clrscr_chitiet();
	void clrscr_ghichu();
	void nhap_nhay(int truoc, int sau);
	void font_nho_i(COLORREF mau);
	void font_to(COLORREF mau);
	void font_chi_tiet(COLORREF mau);
	void venen();
	void ve_tt_dothi();
	void ve_ds_dinh();
	void wellcome();
	//----------------------
	void khoi_tao_prim();
	void prim();
	void thong_bao_prim();
	void khoi_tao_dijkstra();
	void dijkstra();
	void ve_duong_dijkstra();
	void thong_bao_dijkstra();
	BOOL dfs(int u);
	void bfs(int s);
	//------------DATA
	void save(char *name);
	void open(char *name);
	void OnTao();
	void OnChon();
	void OnThoat();
	void OnAbout();
	int trong_dinh(CPoint diem, int boqua);//kiem tra toa do chuot co nam tren dinh nao khong
	void xoa_dinh(int vt);//xoa mot dinh khoi danh sach
	int last_file();
    DECLARE_MESSAGE_MAP()
};


class hop_thoai:public CDialog
{
	CComboBox *cbo;
	CEdit *ed;
	char str[50];
public:
	hop_thoai(UINT id,CWnd *ow):CDialog(id,ow){}
	BOOL OnInitDialog();
	void OnOK();
	DECLARE_MESSAGE_MAP()
};

cua_so::cua_so()
{
	HICON i= chuong_trinh.LoadIcon(IDI_ICON);
	HCURSOR c= chuong_trinh.LoadCursor(IDC_CURSOR);
	HBRUSH f=(HBRUSH)GetStockObject(WHITE_BRUSH);
	LPCSTR d=AfxRegisterWndClass(NULL,c,f,i);
	Create(d,"DEMO-DOTHI",WS_OVERLAPPEDWINDOW,rectDefault,NULL,(LPSTR)IDR_MENU); 
	//-------------------------cua so ao---------------------
	maxX = GetSystemMetrics(SM_CXSCREEN);
	maxY = GetSystemMetrics(SM_CYSCREEN);
	CClientDC DC(this);

	m_memDC.CreateCompatibleDC(&DC);
	m_bmp.CreateCompatibleBitmap(&DC,maxX,maxY);
	m_memDC.SelectObject(&m_bmp);

	m_bkbrush.CreateStockObject(WHITE_BRUSH);
	m_memDC.SelectObject(&m_bkbrush);
	m_memDC.PatBlt(0,0,maxX,maxY,PATCOPY);  

	delay=2000;

	sodinh=0;
	dinh = (CPoint *)malloc(1*sizeof(CPoint));
	e = (int **)malloc(1*sizeof(int*));
	e[0] = (int *)malloc(1*sizeof(int));

	daxet = (int *) malloc(1*sizeof(int));
	truoc = (int *) malloc(1*sizeof(int));
	mangtg = (int *) malloc(1*sizeof(int));
	u = (dt*) malloc(1*sizeof(dt));
	do_thi=0;

	paint = FALSE;
	menu_tao = FALSE;
	them_dinh = FALSE;
	them_cung = FALSE;
	nhan=FALSE;
	bchon_thuat_toan = FALSE;
	bchon_dothi=FALSE;
	hoan_thanh=TRUE;//kiem tra thuat toan da hoan thanh hay chua
	thuat_toan=0;  //bien de nhan biet thuat toan dang duoc chon
	do_thi=0;  //bien de nhan biet do thi nao dang duoc chon
	play=FALSE;
	so_dinh_cu=0;
	h1=LoadLibrary("LIB_DOTHI.dll");
	if(h1==NULL)
	{
		AfxMessageBox("Khong nap duoc thu vien LIB_DOTHI.DLL");
		return;
	}
}

BEGIN_MESSAGE_MAP(cua_so, CFrameWnd)
//    ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_COMMAND(ID_ABOUT,OnAbout)
	ON_COMMAND(ID_TAO,OnTao)
	ON_COMMAND(ID_CHON,OnChon)
	ON_COMMAND(ID_THOAT,OnThoat)
END_MESSAGE_MAP()


BEGIN_MESSAGE_MAP(hop_thoai,CDialog)
	ON_COMMAND(IDOK,OnOK)
END_MESSAGE_MAP()



BOOL ung_dung::InitInstance()
{
	m_pMainWnd = new cua_so();
	m_pMainWnd->ShowWindow(3);
	m_pMainWnd->UpdateWindow();
	return true;
}

int cua_so::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CFrameWnd::OnCreate(lpCreateStruct) == -1)
		   return -1;

	if(!tool.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		  | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC))
	{
		MessageBox("Khong tao duoc toolbars\n");
		return -1;
	}
	tool.LoadToolBar(IDR_TOOLBAR);
	tool.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&tool);
	return 0;
}



void cua_so::OnAbout()
{
	ID_DIALOG=3;
	hop_thoai d2(IDD_ABOUT,this);
	d2.DoModal();
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++BAT CAC SU KIEN++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

afx_msg void cua_so::OnClose()
{
	if(MessageBox("Ban chac chan muon thoat ?","Thong bao",MB_YESNO|MB_ICONQUESTION)==IDYES)
		DestroyWindow();
}

afx_msg void cua_so::OnDestroy()
{
	free(dinh);//mang luu vi tri cua cac dinh
	if(bchon_dothi==TRUE && bchon_thuat_toan==TRUE)
	{
		for(i=0;i<sodinh;i++)
		{
			free(e[i]);
		}
		free(e);
		if(thuat_toan!=1)
		{
			free(daxet);//mang kiem tra da xet
			free(u);//mang kieu dt 
			FreeLibrary(h1); 
			free(truoc);//mang kiem tra dinh truoc no
			free(mangtg);
		}
	}
}


void cua_so::OnPaint()
{
	PAINTSTRUCT ps;
	ngu_canh = BeginPaint(&ps);
		ngu_canh->BitBlt(0,0,maxX,maxY,&m_memDC,0,0,SRCCOPY); 
	EndPaint(&ps);
	if(paint==FALSE)
	{
		venen();
	} 
}


void cua_so::venen()
{
	RECT r;
	ngu_canh=GetDC();
	GetClientRect(&r);
	p.DeleteObject();
	p.CreatePen(PS_SOLID,1,RGB(255,255,255));
	b.DeleteObject();
	b.CreateSolidBrush(RGB(255,255,255));
	ngu_canh->SelectObject(&b);
	m_memDC.SelectObject(&b);
	ngu_canh->SelectObject(&p);
	m_memDC.SelectObject(&p);
	ngu_canh->Rectangle(0,0,r.right,r.bottom);
	m_memDC.Rectangle(0,0,r.right,r.bottom);
	kc=(r.right-1002)/2;
	bit.LoadBitmap(IDB_DAODIEN);
	cdctg.CreateCompatibleDC(ngu_canh);
	cdctg.SelectObject(&bit);
	ngu_canh->BitBlt(kc,0,1002,684,&cdctg,0,0,SRCCOPY);  
	m_memDC.BitBlt(kc,0,1002,684,&cdctg,0,0,SRCCOPY); 
	//	ngu_canh->DeleteDC();
	cdctg.DeleteDC();
	bit.DeleteObject();
	paint = TRUE;
	SetTimer(113,1000,NULL);
	i=0;
}

afx_msg void cua_so::OnLButtonDown(UINT nGlags, CPoint point)
{
	KillTimer(113);
//	sprintf(str,"%d,%d",point.x,point.y);
//	AfxMessageBox(str);
	if(menu_tao==FALSE)
	{
		if(point.x>=kc+152 && point.y>=17 && point.x<=kc+322 && point.y<=65) //Chon do thi
		{
			if(hoan_thanh==FALSE || play==TRUE)
				return;
			bmenu=true;
			bchon_dothi=TRUE;
			clrscr_chitiet();
			sprintf(str,"DATA/DT%d.DAT",do_thi);
			open(str);
			clrscr_main();
			ve_do_thi();
			clrscr_ghichu();
			font_nho_i(RGB(0,0,0));
			sprintf(str,"Do thi DT%d.DAT, co %d dinh",do_thi,sodinh);
			ngu_canh->TextOut(kc+545,465,str);
			m_memDC.TextOut(kc+545,465,str);
			ve_tt_dothi();
			imenu=0;
		}
		else
			if(point.x>=kc+360 && point.y>=17 && point.x<=kc+621 && point.y<=64) //Chon thuat toan
			{
				if(hoan_thanh==FALSE || play==TRUE)
					return;
				bchon_thuat_toan=TRUE;
				clrscr_chitiet();
				clrscr_ghichu();
				imenu=1;
				switch (thuat_toan)
				{
				case 0:
					clrscr_main();
					font_to(RGB(0,0,255));
					ngu_canh->TextOut(kc+370,183,"Dijkstra");
					m_memDC.TextOut(kc+370,183,"Dijkstra");
					clrscr_ghichu();
					font_nho_i(RGB(0,0,0));
					ngu_canh->TextOut(kc+545,465,"Tim duong di ngan nhat");
					m_memDC.TextOut(kc+545,465,"Tim duong di ngan nhat");
					ngu_canh->TextOut(kc+545,485,"tren do thi");
					m_memDC.TextOut(kc+545,485,"tren do thi");
					break;
				case 1:
					clrscr_main();
					font_to(RGB(0,0,255));
					ngu_canh->TextOut(kc+370,183,"Prim");
					m_memDC.TextOut(kc+370,183,"Prim");
					clrscr_ghichu();
					font_nho_i(RGB(0,0,0));
					ngu_canh->TextOut(kc+545,465,"Tim cay khung be nhat");
					m_memDC.TextOut(kc+545,465,"Tim cay khung be nhat");
					break;
				case 2:
					clrscr_main();
					font_to(RGB(0,0,255));
					ngu_canh->TextOut(kc+370,183,"DFS");
					m_memDC.TextOut(kc+370,183,"DFS");
					clrscr_ghichu();
					font_nho_i(RGB(0,0,0));
					ngu_canh->TextOut(kc+545,465,"Tim kiem theo chieu sau");
					m_memDC.TextOut(kc+545,465,"Tim kiem theo chieu sau");
					break;
				case 3:
					clrscr_main();
					font_to(RGB(0,0,255));
					ngu_canh->TextOut(kc+370,183,"BFS");
					m_memDC.TextOut(kc+370,183,"BFS");
					clrscr_ghichu();
					font_nho_i(RGB(0,0,0));
					ngu_canh->TextOut(kc+545,465,"Tim kiem theo chieu rong");
					m_memDC.TextOut(kc+545,465,"Tim kiem theo chieu rong");
					break;
				}
			}
		else
			if(point.x>=kc+918 && point.y>=185 && point.x<=kc+976 && point.y<=250 && (bchon_dothi==TRUE || bchon_thuat_toan==TRUE) && hoan_thanh==TRUE) //tiep
			{
				if(imenu==0)//dang o menu chon do thi
				{
					do_thi++;
					if(do_thi>=last_file())
						do_thi=0;
					clrscr_chitiet();
					sprintf(str,"DATA/DT%d.DAT",do_thi);
					open(str);
					clrscr_main();
					ve_do_thi();
					clrscr_ghichu();
					font_nho_i(RGB(0,0,0));
					sprintf(str,"Do thi DT%d.DAT, co %d dinh",do_thi,sodinh);
					ngu_canh->TextOut(kc+545,465,str);
					m_memDC.TextOut(kc+545,465,str);
					ve_tt_dothi();
				}
				else//dang o menu chon thuat toan
					if(imenu==1)
					{
						font_to(RGB(0,0,255));
						thuat_toan++;
						if(thuat_toan>3)
							thuat_toan=0;
						switch (thuat_toan)
						{
						case 0:
							clrscr_main();
							font_to(RGB(0,0,255));
							ngu_canh->TextOut(kc+370,183,"Dijkstra");
							m_memDC.TextOut(kc+370,183,"Dijkstra");
							clrscr_ghichu();
							font_nho_i(RGB(0,0,0));
							ngu_canh->TextOut(kc+545,465,"Tim duong di ngan nhat");
							m_memDC.TextOut(kc+545,465,"Tim duong di ngan nhat");
							ngu_canh->TextOut(kc+545,485,"tren do thi");
							m_memDC.TextOut(kc+545,485,"tren do thi");
							break;
						case 1:
							clrscr_main();
							font_to(RGB(0,0,255));
							ngu_canh->TextOut(kc+370,183,"Prim");
							m_memDC.TextOut(kc+370,183,"Prim");
							clrscr_ghichu();
							font_nho_i(RGB(0,0,0));
							ngu_canh->TextOut(kc+545,465,"Tim cay khung be nhat");
							m_memDC.TextOut(kc+545,465,"Tim cay khung be nhat");
							break;
						case 2:
							clrscr_main();
							font_to(RGB(0,0,255));
							ngu_canh->TextOut(kc+370,183,"DFS");
							m_memDC.TextOut(kc+370,183,"DFS");
							clrscr_ghichu();
							font_nho_i(RGB(0,0,0));
							ngu_canh->TextOut(kc+545,465,"Tim kiem theo chieu sau");
							m_memDC.TextOut(kc+545,465,"Tim kiem theo chieu sau");
							break;
						case 3:
							clrscr_main();
							font_to(RGB(0,0,255));
							ngu_canh->TextOut(kc+370,183,"BFS");
							m_memDC.TextOut(kc+370,183,"BFS");
							clrscr_ghichu();
							font_nho_i(RGB(0,0,0));
							ngu_canh->TextOut(kc+545,465,"Tim kiem theo chieu rong");
							m_memDC.TextOut(kc+545,465,"Tim kiem theo chieu rong");
							break;
						}
					}
			}
		else
			if(point.x>=kc+23 && point.y>=185 && point.x<=kc+78 && (bchon_dothi==TRUE || bchon_thuat_toan==TRUE) && hoan_thanh==TRUE) //sau
			{
				if(imenu==0)
				{
					do_thi--;
					if(do_thi<0)
						do_thi=last_file()-1;
					clrscr_chitiet();
					sprintf(str,"DATA/DT%d.DAT",do_thi);
					open(str);
					clrscr_main();
					ve_do_thi();
					clrscr_ghichu();
					font_nho_i(RGB(0,0,0));
					sprintf(str,"Do thi DT%d.DAT, co %d dinh",do_thi,sodinh);
					ngu_canh->TextOut(kc+545,465,str);
					m_memDC.TextOut(kc+545,465,str);
					ve_tt_dothi();
				}
				else
					if(imenu==1)
					{
						thuat_toan--;
						if(thuat_toan<0)
							thuat_toan=3;
						switch (thuat_toan)
						{
						case 0:
							clrscr_main();
							font_to(RGB(0,0,255));
							ngu_canh->TextOut(kc+370,183,"Dijkstra");
							m_memDC.TextOut(kc+370,183,"Dijkstra");
							clrscr_ghichu();
							font_nho_i(RGB(0,0,0));
							ngu_canh->TextOut(kc+545,465,"Tim duong di ngan nhat");
							m_memDC.TextOut(kc+545,465,"Tim duong di ngan nhat");
							ngu_canh->TextOut(kc+545,485,"tren do thi");
							m_memDC.TextOut(kc+545,485,"tren do thi");
							break;
						case 1:
							clrscr_main();
							font_to(RGB(0,0,255));
							ngu_canh->TextOut(kc+370,183,"Prim");
							m_memDC.TextOut(kc+370,183,"Prim");
							clrscr_ghichu();
							font_nho_i(RGB(0,0,0));
							ngu_canh->TextOut(kc+545,465,"Tim cay khung be nhat");
							m_memDC.TextOut(kc+545,465,"Tim cay khung be nhat");
							break;
						case 2:
							clrscr_main();
							font_to(RGB(0,0,255));
							ngu_canh->TextOut(kc+370,183,"DFS");
							m_memDC.TextOut(kc+370,183,"DFS");
							clrscr_ghichu();
							font_nho_i(RGB(0,0,0));
							ngu_canh->TextOut(kc+545,465,"Tim kiem theo chieu sau");
							m_memDC.TextOut(kc+545,465,"Tim kiem theo chieu sau");
							break;
						case 3:
							clrscr_main();
							font_to(RGB(0,0,255));
							ngu_canh->TextOut(kc+370,183,"BFS");
							m_memDC.TextOut(kc+370,183,"BFS");
							clrscr_ghichu();
							font_nho_i(RGB(0,0,0));
							ngu_canh->TextOut(kc+545,465,"Tim kiem theo chieu rong");
							m_memDC.TextOut(kc+545,465,"Tim kiem theo chieu rong");
							break;
						}
					}
			}
		else
			if(point.x>=kc+471 && point.y>=372 && point.x<=kc+537 && point.y<=412) //play/pause
			{
				if(play==TRUE)
				{
					play=FALSE;
					CBitmap a;
					a.LoadBitmap(IDB_PLAY2);
					CDC d,*c=GetDC();
					d.CreateCompatibleDC(c);
					d.SelectObject(&a);
					c->BitBlt(kc+471,372,66,40,&d,0,0,SRCCOPY);
					m_memDC.BitBlt(kc+471,372,66,40,&d,0,0,SRCCOPY);
					//-----------
					switch(thuat_toan)
					{
					case 0:
						KillTimer(110);
						hoan_thanh=FALSE;
						break;
					case 1:
						KillTimer(111);
						hoan_thanh=FALSE;
						break;
					}
				}
				else
				{
					if(bchon_dothi==false)
					{
						AfxMessageBox("Ban chua chon do thi !",MB_ICONWARNING);
						return;
					}
					if(bchon_thuat_toan==false)
					{
						AfxMessageBox("Ban chua chon thuat toan !",MB_ICONWARNING);
						return;
					}
					play=TRUE;
					CBitmap a;
					a.LoadBitmap(IDB_PAUSE2);
					CDC d,*c=GetDC();
					d.CreateCompatibleDC(c);
					d.SelectObject(&a);
					c->BitBlt(kc+471,372,70,40,&d,5,0,SRCCOPY);
					m_memDC.BitBlt(kc+471,372,70,40,&d,5,0,SRCCOPY);
					//-------------------
					if(hoan_thanh==TRUE)
					{
						int i;
						for(i=0;i<sodinh;i++)
						{
							daxet[i]=0;
							u[i].a=0;
							u[i].b=vocung;
							mangtg[i]=-1;
							truoc[i]=0;
						}
					}
					//-------------------
					if(thuat_toan==0)
					{
						if(hoan_thanh==TRUE)
						{
							do 
							{
								ID_DIALOG=0;
								hop_thoai d0(ID_CHONDINH,this);
								d0.DoModal();
							} while (chon_sai==TRUE);
							ve_bang();
							khoi_tao_dijkstra();
						}
						SetTimer(110,delay,NULL);
					}
					else
						if(thuat_toan==1)
						{
							if(hoan_thanh==TRUE)
							{
								//---------goi dll------------
								typedef BOOL (*FPointer)(int, dt *, int *, int *, int **, int);
								FPointer fp;
								fp=(FPointer)GetProcAddress(h1,"lien_thong");
								if(fp(s,u,mangtg,daxet,e,sodinh)==FALSE)
								{
									AfxMessageBox("Do thi hien tai khong lien thong !");
									play=FALSE;
									CBitmap a;
									CDC d,*c=GetDC();
									c=GetDC();
									a.LoadBitmap(IDB_PLAY1);
									d.CreateCompatibleDC(c);
									d.SelectObject(&a);
									c->BitBlt(kc+471,372,66,40,&d,0,0,SRCCOPY);
									a.DeleteObject();
									d.DeleteDC();
									c->DeleteDC();
									return;
								}
								do
								{
									ID_DIALOG=1;
									hop_thoai d1(IDD_DINHXUATPHAT,this);
									d1.DoModal();
								}while(chon_sai==TRUE);
								ve_bang();
								khoi_tao_prim();
							}
							SetTimer(111,delay,NULL);
						}
					else
						if(thuat_toan==2)
						{
							if(hoan_thanh==TRUE)
							{
								do
								{
									ID_DIALOG=1;
									hop_thoai d1(IDD_DINHXUATPHAT,this);
									d1.DoModal();
								}while(chon_sai==TRUE);
								int i;
								for(i=0;i<sodinh;i++)
								{
									truoc[i]=s;
								}
								clrscr_chitiet();
								clrscr_ghichu();
								clrscr_main();
								ve_do_thi();
								font_nho_i(RGB(0,0,0));
								xghichu=545;
								yghichu=465;
								yghichu=yghichu+20;
								ngu_canh->TextOut(kc+545,yghichu,"TIM KIEM THEO CHIEU SAU");
								m_memDC.TextOut(kc+545,yghichu,"TIM KIEM THEO CHIEU SAU");
								yghichu=yghichu+20;
								ngu_canh->TextOut(kc+545,yghichu,"Thu tu tim kiem la: ");
								m_memDC.TextOut(kc+545,yghichu,"Thu tu tim kiem la: ");
								yghichu+=20;
								xghichu=kc+545;
								if(dfs(s)==FALSE)
								{
									xghichu=kc+545;
									yghichu+=20;
									font_nho_i(RGB(255,0,0));
									ngu_canh->TextOut(xghichu,yghichu,"Do thi khong lien thong !");
									m_memDC.TextOut(xghichu,yghichu,"Do thi khong lien thong !");
								}
								else
								{
									font_nho_i(RGB(255,0,0));
									yghichu+=20;
									ngu_canh->TextOut(kc+545,yghichu,"Hoan thanh !");
									m_memDC.TextOut(kc+545,yghichu,"Hoan thanh !");
								}
							}
						}
						if(thuat_toan==3)
						{
							if(hoan_thanh==TRUE)
							{
								do
								{
									ID_DIALOG=1;
									hop_thoai d1(IDD_DINHXUATPHAT,this);
									d1.DoModal();
								}while(chon_sai==TRUE);
								clrscr_chitiet();
								clrscr_ghichu();
								clrscr_main();
								ve_do_thi();
								font_nho_i(RGB(0,0,0));
								xghichu=kc+545;
								yghichu=465;
								yghichu=yghichu+20;
								ngu_canh->TextOut(kc+545,yghichu,"TIM KIEM THEO CHIEU RONG");
								m_memDC.TextOut(kc+545,yghichu,"TIM KIEM THEO CHIEU RONG");
								yghichu=yghichu+20;
								ngu_canh->TextOut(kc+545,yghichu,"Thu tu tim kiem la: ");
								m_memDC.TextOut(kc+545,yghichu,"Thu tu tim kiem la: ");
								yghichu+=20;
								xghichu=kc+545;
								bfs(s);
								font_nho_i(RGB(255,0,0));
								yghichu+=20;
								ngu_canh->TextOut(kc+545,yghichu,"Hoan thanh !");
								m_memDC.TextOut(kc+545,yghichu,"Hoan thanh !");
							}
						}
				}
			}
		else
			if(point.x>=kc+351 && point.y>=372 && point.x<=kc+427 && point.y<=413 && play==TRUE) //slow
			{
				delay+=300;
				switch(thuat_toan)
				{
				case 0:
					KillTimer(110);
					SetTimer(110,delay,NULL);
					break;
				case 1:
					KillTimer(111);
					SetTimer(111,delay,NULL);
					break;
				}
			}
		else
			if(point.x>=kc+584 && point.y>=372 && point.x<=kc+658 && point.y<=411 && play==TRUE) //fast
			{
				if(delay>100)
				{
					delay-=300;
					switch(thuat_toan)
					{
					case 0:
						KillTimer(110);
						SetTimer(110,delay,NULL);
						break;
					case 1:
						KillTimer(111);
						SetTimer(111,delay,NULL);
						break;
					}
				}
			}
		else
			if(point.x>=kc+647 && point.y>=17 && point.x<=kc+848 && point.y<=64) //tro giup
			{
				OnThongtin();
			}
	}
	//+++++++++++++++++++++++++++++++++++++++++++++++menu tao do thi+++++++++++++++++++++++++++++++
	else
	{
		if(point.x>=kc+152 && point.y>=17 && point.x<=kc+322 && point.y<=65) //Them dinh
		{
			if(them_dinh==TRUE)
				return;
			if(sodinh>0)
			{
				ve_ds_dinh();
			}
			if(them_cung==TRUE)
			{
				so_dinh_cu=sodinh;
			}
			them_dinh=TRUE;
			them_cung=FALSE;
		}
		else
			if(point.x>=kc+360 && point.y>=17 && point.x<=kc+621 && point.y<=64) //them cung
			{
				if(them_cung==TRUE)
					return;
				if(sodinh<=1)
				{
					AfxMessageBox("Ban phai tao it nhat 2 dinh !",MB_ICONWARNING);
					return;
				}
				else
				{
					them_cung=TRUE;
					them_dinh=FALSE;
					clrscr_main();
					if(so_dinh_cu>0)
					{
						for(i=0;i<so_dinh_cu-1;i++)
						{
							if(e[i][i+1]>0)
								ve_cung(i,i+1,RGB(0,0,0),RGB(0,0,0));
						}
					}
					ve_ds_dinh();
					e=(int **) realloc(e,sodinh*sizeof(int*));
					if(so_dinh_cu==0)
					{
						e[0]=(int *) realloc(e[0],sodinh*sizeof(int));
						for(i=1;i<sodinh;i++)
						{
							e[i]=(int *) malloc(sodinh*sizeof(int));
						}
						for(i=0;i<sodinh;i++)
						{
							for(int j=0;j<sodinh;j++)
							{
								if(i==j)
									e[i][j]=0;
								else
									e[i][j]=-1;
							}
						}
					}
					else
					{
						for(i=0;i<so_dinh_cu;i++)
						{
							e[i]=(int *) realloc(e[i],sodinh*sizeof(int));
						}
						for(i=so_dinh_cu;i<sodinh;i++)
						{
							e[i]=(int *) malloc(sodinh*sizeof(int));
						}
						for(i=so_dinh_cu;i<sodinh;i++)
						{
							for(int j=so_dinh_cu;j<sodinh;j++)
							{
								if(i==j)
									e[i][j]=0;
								else
									e[i][j]=-1;
							}
						}
					}
					ve_tt_dothi();
				}
			}
		else
			if(point.x>=kc+180 && point.y>=110 && point.x<=kc+810 && point.y<=300) //vung man hinh chinh
			{
				if(them_dinh==TRUE)
				{
					dinh_nhan=trong_dinh(point,-1);
					if(dinh_nhan==-1)
					{
						if(sodinh<11)
						{
							sodinh++;
							sprintf(str,"%c",sodinh-1+65);
							ve_dinh(point,RGB(0,128,0),RGB(0,0,0),RGB(255,255,255),str);
							dinh=(CPoint *) realloc(dinh,sodinh*sizeof(CPoint));
							dinh[sodinh-1]=point;
						}
					}
					else
					{
						nhan=TRUE;
					}
				}
				else
					if(them_cung==TRUE)
					{
						dinh_nhan=trong_dinh(point,-1);
						if(dinh_nhan!=-1)
						{
							nhan=TRUE;
						}
					}
			}
		else
			if(point.x>=kc+647 && point.y>=17 && point.x<=kc+848 && point.y<=64) //thoat
			{
				if(sodinh>0)
					if(MessageBox("Co luu do thi vua tao ?","Luu do thi",MB_YESNO|MB_ICONQUESTION)==IDYES)
					{
						i=last_file();
						sprintf(str,"DATA/DT%d.DAT",i);
						save(str);
					}
				venen();
				menu_tao=FALSE;
			}
	}
}


afx_msg void cua_so::OnLButtonUp(UINT nGlags, CPoint point)
{
	if(menu_tao==TRUE)
	{
		if(them_dinh==TRUE)
		{
			if(nhan==TRUE)
			{
				if(point.x>=kc+180 && point.y>=110 && point.x<=kc+810 && point.y<=300) //vung man hinh chinh
				{
					if(trong_dinh(point,dinh_nhan)==-1)	
					{
						ve_dinh(dinh[dinh_nhan],RGB(255,255,255),RGB(255,255,255),RGB(255,255,255),"");
						sprintf(str,"%c",dinh_nhan+65);
						ve_dinh(point,RGB(0,128,0),RGB(0,0,0),RGB(255,255,255),str);
						dinh[dinh_nhan]=point;
						nhan=FALSE;
					}
				}
				else
				{
					ve_dinh(dinh[dinh_nhan],RGB(255,255,255),RGB(255,255,255),RGB(255,255,255),"");
					xoa_dinh(dinh_nhan);
					ve_ds_dinh();
					nhan=FALSE;
				}
			}
		}
		else
			if(them_cung==TRUE && point.x>=kc+180 && point.y>=110 && point.x<=kc+810 && point.y<=300)
			{
				if(nhan==TRUE)
				{
					int dinh_den;
					dinh_den=trong_dinh(point,-1);
					if(dinh_den!=-1 && dinh_den!=dinh_nhan)
					{
						do 
						{
							ID_DIALOG=2;
							hop_thoai d2(IDD_NOI,this);
							if(d2.DoModal()==IDOK)
							{
								if(chon_sai==FALSE)
								{
									e[dinh_nhan][dinh_den]=trongso;
									e[dinh_den][dinh_nhan]=trongso;
									ve_cung(dinh_nhan,dinh_den,RGB(0,0,0),RGB(0,0,0));
									sprintf(str,"%c",dinh_nhan+65);
									ve_dinh(dinh[dinh_nhan],RGB(0,128,0),RGB(0,0,0),RGB(255,255,255),str);
									sprintf(str,"%c",dinh_den+65);
									ve_dinh(dinh[dinh_den],RGB(0,128,0),RGB(0,0,0),RGB(255,255,255),str);
									ve_tt_dothi();
								}
							}
						} while (chon_sai==TRUE);
					}
					nhan=FALSE;
				}
			}
	}
}


afx_msg void cua_so::OnMouseMove(UINT nGlags, CPoint point)
{
	if(menu_tao==FALSE)
	{
		if(point.x>=kc+152 && point.y>=17 && point.x<=kc+322 && point.y<=65) //Chon do thi
		{
			CBitmap a;
			a.LoadBitmap(IDB_CHONDOTHI2);
			CDC d,*c=GetDC();
			d.CreateCompatibleDC(c);
			d.SelectObject(&a);
			c->BitBlt(kc+152,17,180,48,&d,0,0,SRCCOPY);
			m_memDC.BitBlt(kc+152,17,180,48,&d,0,0,SRCCOPY);
			HCURSOR hCursor = AfxGetApp()->LoadCursor(IDC_CURSOR1);
			SetCursor(hCursor);
		}
		else
			if(point.x>=kc+360 && point.y>=17 && point.x<=kc+621 && point.y<=64) //Chon thuat toan
			{
				CBitmap a;
				a.LoadBitmap(IDB_CHONTHUATTOAN2);
				CDC d,*c=GetDC();
				d.CreateCompatibleDC(c);
				d.SelectObject(&a);
				c->BitBlt(kc+360,17,261,47,&d,0,0,SRCCOPY);
				m_memDC.BitBlt(kc+360,17,261,47,&d,0,0,SRCCOPY);
				HCURSOR hCursor = AfxGetApp()->LoadCursor(IDC_CURSOR1);
				SetCursor(hCursor);
			}
		else
			if(point.x>=kc+647 && point.y>=17 && point.x<=kc+848 && point.y<=64) //tro giup
			{
				CBitmap a;
				a.LoadBitmap(IDB_TROGIUP2);
				CDC d,*c=GetDC();
				d.CreateCompatibleDC(c);
				d.SelectObject(&a);
				c->BitBlt(kc+647,17,201,47,&d,0,0,SRCCOPY);
				m_memDC.BitBlt(kc+647,17,201,47,&d,0,0,SRCCOPY);
				HCURSOR hCursor = AfxGetApp()->LoadCursor(IDC_CURSOR1);
				SetCursor(hCursor);
			}
		else
			if(point.x>=kc+23 && point.y>=185 && point.x<=kc+78 && point.y<=246) //sau
			{
				CBitmap a;
				a.LoadBitmap(IDB_SAU2);
				CDC d,*c=GetDC();
				d.CreateCompatibleDC(c);
				d.SelectObject(&a);
				c->BitBlt(kc+23,185,55,61,&d,0,0,SRCCOPY);
				m_memDC.BitBlt(kc+23,185,55,61,&d,0,0,SRCCOPY);
				HCURSOR hCursor = AfxGetApp()->LoadCursor(IDC_CURSOR1);
				SetCursor(hCursor);
			}
		else
			if(point.x>=kc+918 && point.y>=185 && point.x<=kc+976 && point.y<=250) //tiep
			{
				CBitmap a;
				a.LoadBitmap(IDB_TIEP2);
				CDC d,*c=GetDC();
				d.CreateCompatibleDC(c);
				d.SelectObject(&a);
				c->BitBlt(kc+918,185,58,65,&d,0,0,SRCCOPY);
				m_memDC.BitBlt(kc+918,185,58,65,&d,0,0,SRCCOPY);
				HCURSOR hCursor = AfxGetApp()->LoadCursor(IDC_CURSOR1);
				SetCursor(hCursor);
			}
		else
			if(point.x>=kc+351 && point.y>=372 && point.x<=kc+427 && point.y<=413) //slow
			{
				CBitmap a;
				a.LoadBitmap(IDB_SLOW2);
				CDC d,*c=GetDC();
				d.CreateCompatibleDC(c);
				d.SelectObject(&a);
				c->BitBlt(kc+351,372,76,41,&d,0,0,SRCCOPY);
				m_memDC.BitBlt(kc+351,372,76,41,&d,0,0,SRCCOPY);
				HCURSOR hCursor = AfxGetApp()->LoadCursor(IDC_CURSOR1);
				SetCursor(hCursor);
			}
		else
			if(point.x>=kc+471 && point.y>=372 && point.x<=kc+537 && point.y<=412) //play/pause
			{
				if(play==FALSE)
				{
					CBitmap a;
					a.LoadBitmap(IDB_PLAY2);
					CDC d,*c=GetDC();
					d.CreateCompatibleDC(c);
					d.SelectObject(&a);
					c->BitBlt(kc+471,372,66,40,&d,0,0,SRCCOPY);
					m_memDC.BitBlt(kc+471,372,66,40,&d,0,0,SRCCOPY);
					HCURSOR hCursor = AfxGetApp()->LoadCursor(IDC_CURSOR1);
					SetCursor(hCursor);
				}
				else
				{
					CBitmap a;
					a.LoadBitmap(IDB_PAUSE2);
					CDC d,*c=GetDC();
					d.CreateCompatibleDC(c);
					d.SelectObject(&a);
					c->BitBlt(kc+471,372,70,40,&d,5,0,SRCCOPY);
					m_memDC.BitBlt(kc+471,372,70,40,&d,5,0,SRCCOPY);
					HCURSOR hCursor = AfxGetApp()->LoadCursor(IDC_CURSOR1);
					SetCursor(hCursor);
				}
			}
		else
			if(point.x>=kc+584 && point.y>=372 && point.x<=kc+658 && point.y<=411) //fast
			{			
				CBitmap a;
				a.LoadBitmap(IDB_FAST2);
				CDC d,*c=GetDC();
				d.CreateCompatibleDC(c);
				d.SelectObject(&a);
				c->BitBlt(kc+584,372,74,39,&d,0,0,SRCCOPY);
				m_memDC.BitBlt(kc+584,372,74,39,&d,0,0,SRCCOPY);
				HCURSOR hCursor = AfxGetApp()->LoadCursor(IDC_CURSOR1);
				SetCursor(hCursor);
			}
		else
		{
			ve_lai_nut();
		}
	}
	else
	{
		if(point.x>=kc+152 && point.y>=17 && point.x<=kc+322 && point.y<=65) //them dinh
		{
			CBitmap a;
			a.LoadBitmap(IDB_THEMDINH2);
			CDC d,*c=GetDC();
			d.CreateCompatibleDC(c);
			d.SelectObject(&a);
			c->BitBlt(kc+160,15,180,48,&d,0,0,SRCCOPY);
			m_memDC.BitBlt(kc+160,15,180,48,&d,0,0,SRCCOPY);
			HCURSOR hCursor = AfxGetApp()->LoadCursor(IDC_CURSOR1);
			SetCursor(hCursor);
		}
		else
			if(point.x>=kc+360 && point.y>=17 && point.x<=kc+621 && point.y<=64) //them cung
			{
				CBitmap a;
				a.LoadBitmap(IDB_THEMCUNG2);
				CDC d,*c=GetDC();
				d.CreateCompatibleDC(c);
				d.SelectObject(&a);
				c->BitBlt(kc+365,15,261,47,&d,0,0,SRCCOPY);
				m_memDC.BitBlt(kc+365,15,261,47,&d,0,0,SRCCOPY);
				HCURSOR hCursor = AfxGetApp()->LoadCursor(IDC_CURSOR1);
				SetCursor(hCursor);
			}
		else
			if(point.x>=kc+647 && point.y>=17 && point.x<=kc+848 && point.y<=64) //thoat
			{
				CBitmap a;
				a.LoadBitmap(IDB_THOAT2);
				CDC d,*c=GetDC();
				d.CreateCompatibleDC(c);
				d.SelectObject(&a);
				c->BitBlt(kc+655,15,201,47,&d,0,0,SRCCOPY);
				m_memDC.BitBlt(kc+655,15,201,47,&d,0,0,SRCCOPY);
				HCURSOR hCursor = AfxGetApp()->LoadCursor(IDC_CURSOR1);
				SetCursor(hCursor);
			}
		else
			if(point.x>=kc+180 && point.y>=110 && point.x<=kc+810 && point.y<=300) //vung man hinh chinh
			{
				if(them_dinh==TRUE)
				{
					if(trong_dinh(point,-1)!=-1)
					{
						HCURSOR hCursor = AfxGetApp()->LoadCursor(IDC_DICHUYEN);
						SetCursor(hCursor);
					}
					else
					{
						if(nhan==FALSE)
						{
							HCURSOR hCursor = AfxGetApp()->LoadCursor(IDC_CO);
							SetCursor(hCursor);
						}
						else
						{
							HCURSOR hCursor = AfxGetApp()->LoadCursor(IDC_DICHUYEN);
							SetCursor(hCursor);
						}
					}
				}
				else
					if(them_cung==TRUE)
					{
						if(trong_dinh(point,-1)!=-1)
						{
							HCURSOR hCursor = AfxGetApp()->LoadCursor(IDC_NOI);
							SetCursor(hCursor);
						}
						else
						{
							if(nhan==TRUE)
							{
								HCURSOR hCursor = AfxGetApp()->LoadCursor(IDC_NOI);
								SetCursor(hCursor);
							}
							else
							{
								HCURSOR hCursor = AfxGetApp()->LoadCursor(IDC_CURSOR);
								SetCursor(hCursor);
							}
						}
					}
			}
		else
		{
			ve_lai_nut();
		}
	}
	
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++HAM PHUC VU CHO DAO DIEN+++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



void cua_so::ve_lai_nut()
{
	HCURSOR hCursor = AfxGetApp()->LoadCursor(IDC_CURSOR);
	SetCursor(hCursor);
	CBitmap a;
	if(menu_tao==FALSE)
	{
		//---------chon do thi
		a.LoadBitmap(IDB_CHONDOTHI1);
		CDC d,*c=GetDC();
		d.CreateCompatibleDC(c);
		d.SelectObject(&a);
		c->BitBlt(kc+152,17,180,48,&d,0,0,SRCCOPY);
		m_memDC.BitBlt(kc+152,17,180,48,&d,0,0,SRCCOPY);
		a.DeleteObject();
		d.DeleteDC();
		c->DeleteDC();
		//--------chon thuat toan
		c=GetDC();
		a.LoadBitmap(IDB_CHONTHUATTOAN1);
		d.CreateCompatibleDC(c);
		d.SelectObject(&a);
		c->BitBlt(kc+360,17,261,47,&d,0,0,SRCCOPY);
		m_memDC.BitBlt(kc+360,17,261,47,&d,0,0,SRCCOPY);
		a.DeleteObject();
		d.DeleteDC();
		c->DeleteDC();
		//--------tro giup
		c=GetDC();
		a.LoadBitmap(IDB_TROGIUP1);
		d.CreateCompatibleDC(c);
		d.SelectObject(&a);
		c->BitBlt(kc+647,17,201,47,&d,0,0,SRCCOPY);
		m_memDC.BitBlt(kc+647,17,201,47,&d,0,0,SRCCOPY);
		a.DeleteObject();
		d.DeleteDC();
		c->DeleteDC();
		//--------sau
		c=GetDC();
		a.LoadBitmap(IDB_SAU1);
		d.CreateCompatibleDC(c);
		d.SelectObject(&a);
		c->BitBlt(kc+23,185,55,61,&d,0,0,SRCCOPY);
		m_memDC.BitBlt(kc+23,185,55,61,&d,0,0,SRCCOPY);
		a.DeleteObject();
		d.DeleteDC();
		c->DeleteDC();
		//--------tiep
		c=GetDC();
		a.LoadBitmap(IDB_TIEP1);
		d.CreateCompatibleDC(c);
		d.SelectObject(&a);
		c->BitBlt(kc+918,185,58,65,&d,0,0,SRCCOPY);
		m_memDC.BitBlt(kc+918,185,58,65,&d,0,0,SRCCOPY);
		a.DeleteObject();
		d.DeleteDC();
		c->DeleteDC();
		//--------slow
		c=GetDC();
		a.LoadBitmap(IDB_SLOW1);
		d.CreateCompatibleDC(c);
		d.SelectObject(&a);
		c->BitBlt(kc+351,372,76,41,&d,0,0,SRCCOPY);
		m_memDC.BitBlt(kc+351,372,76,41,&d,0,0,SRCCOPY);
		a.DeleteObject();
		d.DeleteDC();
		c->DeleteDC();
		//--------play
		if(play==FALSE)
		{
			c=GetDC();
			a.LoadBitmap(IDB_PLAY1);
			d.CreateCompatibleDC(c);
			d.SelectObject(&a);
			c->BitBlt(kc+471,372,66,40,&d,0,0,SRCCOPY);
			m_memDC.BitBlt(kc+471,372,66,40,&d,0,0,SRCCOPY);
			a.DeleteObject();
			d.DeleteDC();
			c->DeleteDC();
		}
		else
		{
			c=GetDC();
			a.LoadBitmap(IDB_PAUSE1);
			d.CreateCompatibleDC(c);
			d.SelectObject(&a);
			c->BitBlt(kc+471,372,70,40,&d,5,0,SRCCOPY);
			m_memDC.BitBlt(kc+471,372,70,40,&d,5,0,SRCCOPY);
			a.DeleteObject();
			d.DeleteDC();
			c->DeleteDC();
		}
		//--------fast
		c=GetDC();
		a.LoadBitmap(IDB_FAST1);
		d.CreateCompatibleDC(c);
		d.SelectObject(&a);
		c->BitBlt(kc+584,372,74,39,&d,0,0,SRCCOPY);
		m_memDC.BitBlt(kc+584,372,74,39,&d,0,0,SRCCOPY);
		a.DeleteObject();
		d.DeleteDC();
		c->DeleteDC();
	}
	else
	{
		//---------them dinh
		a.LoadBitmap(IDB_THEMDINH1);
		CDC d,*c=GetDC();
		d.CreateCompatibleDC(c);
		d.SelectObject(&a);
		c->BitBlt(kc+160,15,180,48,&d,0,0,SRCCOPY);
		m_memDC.BitBlt(kc+160,15,180,48,&d,0,0,SRCCOPY);
		a.DeleteObject();
		d.DeleteDC();
		c->DeleteDC();
		//--------them cung
		c=GetDC();
		a.LoadBitmap(IDB_THEMCUNG1);
		d.CreateCompatibleDC(c);
		d.SelectObject(&a);
		c->BitBlt(kc+365,15,261,47,&d,0,0,SRCCOPY);
		m_memDC.BitBlt(kc+365,15,261,47,&d,0,0,SRCCOPY);
		a.DeleteObject();
		d.DeleteDC();
		c->DeleteDC();
		//--------thoat
		c=GetDC();
		a.LoadBitmap(IDB_THOAT1);
		d.CreateCompatibleDC(c);
		d.SelectObject(&a);
		c->BitBlt(kc+655,15,201,47,&d,0,0,SRCCOPY);
		m_memDC.BitBlt(kc+655,15,201,47,&d,0,0,SRCCOPY);
		a.DeleteObject();
		d.DeleteDC();
		c->DeleteDC();
	}
}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++HAM PHUC VU CHO GIAI THUAT+++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++HAM PHUC VU CHO VE DO THI++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void cua_so::ve_dinh(CPoint vt, COLORREF mn,COLORREF md, COLORREF mc, char *hien)
{
	ngu_canh=GetDC();
	font.DeleteObject();
	font.CreateFont(20,10,0,0,700,0,0,0,0,4,0,1,1,"fithou");
	ngu_canh->SetBkMode(TRANSPARENT);
	m_memDC.SetBkMode(TRANSPARENT);
	ngu_canh->SetTextColor(mc);
	m_memDC.SetTextColor(mc);
	ngu_canh->SetBkColor(RGB(255,255,255));
	m_memDC.SetBkColor(RGB(255,255,255));
	p.DeleteObject();
	p.CreatePen(PS_SOLID,4,md);
	b.DeleteObject();
	b.CreateSolidBrush(mn);
	ngu_canh->SelectObject(&b);
	m_memDC.SelectObject(&b);
	ngu_canh->SelectObject(&p);
	m_memDC.SelectObject(&p);
	ngu_canh->SelectObject(&font);
	m_memDC.SelectObject(&font);
	ngu_canh->Ellipse(vt.x-20,vt.y-20,vt.x+20,vt.y+20);
	m_memDC.Ellipse(vt.x-20,vt.y-20,vt.x+20,vt.y+20);
	ngu_canh->TextOut(vt.x-5,vt.y-10,hien);
	m_memDC.TextOut(vt.x-5,vt.y-10,hien);
}


void cua_so::ve_cung(int truoc, int sau, COLORREF md, COLORREF mc)
{
	ngu_canh=GetDC();
	font.DeleteObject();
	font.CreateFont(20,10,0,0,700,1,0,0,0,4,0,1,1,"fithou");
	ngu_canh->SetBkMode(TRANSPARENT);
	m_memDC.SetBkMode(TRANSPARENT);
	ngu_canh->SetTextColor(mc);
	m_memDC.SetTextColor(mc);
	p.DeleteObject();
	p.CreatePen(PS_SOLID,4,md);
	ngu_canh->SelectObject(&p);
	m_memDC.SelectObject(&p);
	ngu_canh->MoveTo(dinh[truoc]);
	m_memDC.MoveTo(dinh[truoc]);
	ngu_canh->LineTo(dinh[sau]);
	m_memDC.LineTo(dinh[sau]);
	p.DeleteObject();
	p.CreatePen(PS_SOLID,1,RGB(255,255,255));
	b.DeleteObject();
	b.CreateSolidBrush(RGB(255,255,255));
	ngu_canh->SelectObject(&b);
	m_memDC.SelectObject(&b);
	ngu_canh->SelectObject(&p);
	m_memDC.SelectObject(&p);
	if(e[truoc][sau]>=10)
	{
		ngu_canh->Rectangle((dinh[truoc].x+dinh[sau].x)/2-7,((dinh[truoc].y+dinh[sau].y)/2-10),(dinh[truoc].x+dinh[sau].x)/2+10,((dinh[truoc].y+dinh[sau].y)/2+10));
		m_memDC.Rectangle((dinh[truoc].x+dinh[sau].x)/2-7,((dinh[truoc].y+dinh[sau].y)/2-10),(dinh[truoc].x+dinh[sau].x)/2+10,((dinh[truoc].y+dinh[sau].y)/2+10));
	}
	else
	{
		ngu_canh->Rectangle((dinh[truoc].x+dinh[sau].x)/2-7,((dinh[truoc].y+dinh[sau].y)/2-10),(dinh[truoc].x+dinh[sau].x)/2,((dinh[truoc].y+dinh[sau].y)/2+10));
		m_memDC.Rectangle((dinh[truoc].x+dinh[sau].x)/2-7,((dinh[truoc].y+dinh[sau].y)/2-10),(dinh[truoc].x+dinh[sau].x)/2,((dinh[truoc].y+dinh[sau].y)/2+10));
	}
	sprintf(str,"%d",e[truoc][sau]);
	ngu_canh->SelectObject(&font);
	m_memDC.SelectObject(&font);
	ngu_canh->TextOut((dinh[truoc].x+dinh[sau].x)/2-10,((dinh[truoc].y+dinh[sau].y)/2)-10,str);
	m_memDC.TextOut((dinh[truoc].x+dinh[sau].x)/2-10,((dinh[truoc].y+dinh[sau].y)/2)-10,str);
}

void cua_so::ve_do_thi()
{
	int i,j;
	for(i=0;i<sodinh;i++)
	{
		for(j=0;j<=i;j++)
		{
			if(e[i][j]>0)
				ve_cung(i,j,RGB(0,0,0),RGB(0,0,0));
		}
	}
	for(i=0;i<sodinh;i++)
	{
		sprintf(str,"%c",i+65);
		ve_dinh(dinh[i],RGB(255,255,255),RGB(0,0,0),RGB(0,0,0),str);
	}
}

void cua_so::ve_bang()
{
	int x,y,i;
	x=kc+175;y=465;
	clrscr_chitiet();
	//ve bang mo ta chi tiet
	p.DeleteObject();
	p.CreatePen(PS_SOLID,1,RGB(0,0,0));
	b.DeleteObject();
	b.CreateSolidBrush(RGB(255,255,255));
	font.DeleteObject();
	font.CreateFont(20,10,0,0,700,1,0,0,0,4,0,1,1,"fithou");
	ngu_canh->SetBkMode(TRANSPARENT);
	m_memDC.SetBkMode(TRANSPARENT);
	ngu_canh->SetTextColor(RGB(0,0,0));
	m_memDC.SetTextColor(RGB(0,0,0));
	ngu_canh->SelectObject(font);
	m_memDC.SelectObject(font);
	ngu_canh->SelectObject(p);
	m_memDC.SelectObject(p);
	ngu_canh->SelectObject(b);
	m_memDC.SelectObject(b);
	
	for(i=0;i<=sodinh+1;i++)//ve cot
	{
		ngu_canh->MoveTo(x,y);
		m_memDC.MoveTo(x,y);
		ngu_canh->LineTo(x,y+((sodinh+1)*(180/(sodinh+1))));
		m_memDC.LineTo(x,y+((sodinh+1)*(180/(sodinh+1))));
		x=x+(302/(sodinh+1));
		if(i<sodinh)
		{
			sprintf(str,"%c",i+65);
			ngu_canh->TextOut(x+10,y+2,str);
			m_memDC.TextOut(x+10,y+2,str);
		}
	}
	x=kc+175;y=465;
	for(i=0;i<=sodinh+1;i++)//ve hang
	{
		ngu_canh->MoveTo(x,y);
		m_memDC.MoveTo(x,y);
		ngu_canh->LineTo(x+((sodinh+1)*(302/(sodinh+1))),y);
		m_memDC.LineTo(x+((sodinh+1)*(302/(sodinh+1))),y);
		y=y+(180/(sodinh+1));
		if(i<sodinh)
		{
			sprintf(str,"%d",i+1);
			ngu_canh->TextOut(x+10,y+2,str);
			m_memDC.TextOut(x+10,y+2,str);
		}
	}
}


void cua_so::clrscr_ghichu()
{
	ngu_canh=GetDC();
	p.DeleteObject();
	p.CreatePen(PS_SOLID,4,RGB(255,255,255));
	b.DeleteObject();
	b.CreateSolidBrush(RGB(255,255,255));
	ngu_canh->SelectObject(&b);
	m_memDC.SelectObject(&b);
	ngu_canh->SelectObject(&p);
	m_memDC.SelectObject(&p);
	ngu_canh->Rectangle(kc+520,464,kc+860,653);
	m_memDC.Rectangle(kc+520,464,kc+860,653);
}

void cua_so::clrscr_main()
{
	ngu_canh=GetDC();
	p.DeleteObject();
	p.CreatePen(PS_SOLID,4,RGB(255,255,255));
	b.DeleteObject();
	b.CreateSolidBrush(RGB(255,255,255));
	ngu_canh->SelectObject(&b);
	m_memDC.SelectObject(&b);
	ngu_canh->SelectObject(&p);
	m_memDC.SelectObject(&p);
	ngu_canh->Rectangle(kc+156,100,kc+847,340);
	m_memDC.Rectangle(kc+156,95,kc+847,337);
}


void cua_so::clrscr_chitiet()
{
	//xoa man hinh chi tiet thuat toan
	ngu_canh=GetDC();
	p.DeleteObject();
	p.CreatePen(PS_SOLID,1,RGB(255,255,255));
	b.DeleteObject();
	b.CreateSolidBrush(RGB(255,255,255));
	ngu_canh->SelectObject(&b);
	m_memDC.SelectObject(&b);
	ngu_canh->SelectObject(&p);
	m_memDC.SelectObject(&p);
	ngu_canh->Rectangle(kc+146,464,kc+491,653);
	m_memDC.Rectangle(kc+146,464,kc+491,653);
}

afx_msg void cua_so::OnTimer(UINT nIDEvent)
{
	switch(nIDEvent)
	{
	case 110:
		dijkstra();
		break;
	case 111:
		prim();
		break;
	case 113:
		wellcome();
		break;
	}
	
}

void cua_so::font_nho_i(COLORREF mau)
{
	ngu_canh=GetDC();
	font.DeleteObject();
	font.CreateFont(20,10,0,0,700,1,0,0,0,4,0,1,1,"font_nho");
	ngu_canh->SetBkMode(TRANSPARENT);
	m_memDC.SetBkMode(TRANSPARENT);
	ngu_canh->SetTextColor(mau);
	m_memDC.SetTextColor(mau);
	ngu_canh->SelectObject(font);
	m_memDC.SelectObject(font);
}

void cua_so::font_to(COLORREF mau)
{
	ngu_canh=GetDC();
	font.DeleteObject();
	font.CreateFont(70,50,0,0,400,0,0,0,0,0,0,0,0,"font_to");
	ngu_canh->SetBkMode(TRANSPARENT);
	m_memDC.SetBkMode(TRANSPARENT);
	ngu_canh->SetTextColor(mau);
	m_memDC.SetTextColor(mau);
	ngu_canh->SelectObject(font);
	m_memDC.SelectObject(font);
}

void cua_so::font_chi_tiet(COLORREF mau)
{
	ngu_canh=GetDC();
	font.DeleteObject();
	font.CreateFont(15,8,0,0,700,1,0,0,0,4,0,1,1,"font_chi_tiet");
	ngu_canh->SetBkMode(TRANSPARENT);
	m_memDC.SetBkMode(TRANSPARENT);
	ngu_canh->SetTextColor(mau);
	m_memDC.SetTextColor(mau);
	ngu_canh->SelectObject(font);
	m_memDC.SelectObject(font);
}

void cua_so::nhap_nhay(int truoc , int sau)
{
	ve_cung(truoc,sau,RGB(255,255,0),RGB(0,128,0));
	sprintf(str,"%c",truoc+65);
	ve_dinh(dinh[truoc],RGB(255,0,0),RGB(0,0,0),RGB(255,255,255),str);
	sprintf(str,"%c",sau+65);
	ve_dinh(dinh[sau],RGB(255,255,255),RGB(255,255,0),RGB(255,0,0),str);

	font_nho_i(RGB(0,0,255));
	sprintf(str,"%d",e[truoc][sau]);
	ngu_canh->TextOut((dinh[truoc].x+dinh[sau].x)/2-10,((dinh[truoc].y+dinh[sau].y)/2)-10,str);
	m_memDC.TextOut((dinh[truoc].x+dinh[sau].x)/2-10,((dinh[truoc].y+dinh[sau].y)/2)-10,str);
	Sleep(delay/4);
	font_nho_i(RGB(255,0,0));
	sprintf(str,"%d",e[truoc][sau]);
	ngu_canh->TextOut((dinh[truoc].x+dinh[sau].x)/2-10,((dinh[truoc].y+dinh[sau].y)/2)-10,str);
	m_memDC.TextOut((dinh[truoc].x+dinh[sau].x)/2-10,((dinh[truoc].y+dinh[sau].y)/2)-10,str);
	Sleep(delay/4);
	font_nho_i(RGB(0,0,255));
	sprintf(str,"%d",e[truoc][sau]);
	ngu_canh->TextOut((dinh[truoc].x+dinh[sau].x)/2-10,((dinh[truoc].y+dinh[sau].y)/2)-10,str);
	m_memDC.TextOut((dinh[truoc].x+dinh[sau].x)/2-10,((dinh[truoc].y+dinh[sau].y)/2)-10,str);
	Sleep(delay/4);
	font_nho_i(RGB(255,0,0));
	sprintf(str,"%d",e[truoc][sau]);
	ngu_canh->TextOut((dinh[truoc].x+dinh[sau].x)/2-10,((dinh[truoc].y+dinh[sau].y)/2)-10,str);
	m_memDC.TextOut((dinh[truoc].x+dinh[sau].x)/2-10,((dinh[truoc].y+dinh[sau].y)/2)-10,str);
	Sleep(delay/4);
	font_nho_i(RGB(0,0,0));
	sprintf(str,"%d",e[truoc][sau]);
	ngu_canh->TextOut((dinh[truoc].x+dinh[sau].x)/2-10,((dinh[truoc].y+dinh[sau].y)/2)-10,str);
	m_memDC.TextOut((dinh[truoc].x+dinh[sau].x)/2-10,((dinh[truoc].y+dinh[sau].y)/2)-10,str);
}


void cua_so::ve_tt_dothi()
{
	//-------ve bang------------
	int x,y,i,j;
	x=kc+175;y=465;
	clrscr_chitiet();
	//ve bang mo ta chi tiet
	p.DeleteObject();
	p.CreatePen(PS_SOLID,1,RGB(0,0,0));
	b.DeleteObject();
	b.CreateSolidBrush(RGB(255,255,255));
	font.DeleteObject();
	font.CreateFont(20,10,0,0,700,1,0,0,0,4,0,1,1,"fithou");
	ngu_canh->SetBkMode(TRANSPARENT);
	m_memDC.SetBkMode(TRANSPARENT);
	ngu_canh->SetTextColor(RGB(0,0,0));
	m_memDC.SetTextColor(RGB(0,0,0));
	ngu_canh->SelectObject(font);
	m_memDC.SelectObject(font);
	ngu_canh->SelectObject(p);
	m_memDC.SelectObject(p);
	ngu_canh->SelectObject(b);
	m_memDC.SelectObject(b);
	
	for(i=0;i<=sodinh+1;i++)//ve cot
	{
		ngu_canh->MoveTo(x,y);
		m_memDC.MoveTo(x,y);
		ngu_canh->LineTo(x,y+((sodinh+1)*(180/(sodinh+1))));
		m_memDC.LineTo(x,y+((sodinh+1)*(180/(sodinh+1))));
		x=x+(302/(sodinh+1));
		if(i<sodinh)
		{
			sprintf(str,"%c",i+65);
			ngu_canh->TextOut(x+10,y+2,str);
			m_memDC.TextOut(x+10,y+2,str);
		}
	}
	x=kc+175;y=465;
	for(i=0;i<=sodinh+1;i++)//ve hang
	{
		ngu_canh->MoveTo(x,y);
		m_memDC.MoveTo(x,y);
		ngu_canh->LineTo(x+((sodinh+1)*(302/(sodinh+1))),y);
		m_memDC.LineTo(x+((sodinh+1)*(302/(sodinh+1))),y);
		y=y+(180/(sodinh+1));
		if(i<sodinh)
		{
			sprintf(str,"%c",i+65);
			ngu_canh->TextOut(x+10,y+2,str);
			m_memDC.TextOut(x+10,y+2,str);
		}
	}
	//-------ve chi tiet do thi------
	font_chi_tiet(RGB(0,0,0));
	xchitiet=kc+175;
	ychitiet=465;
	for(i=0;i<sodinh;i++)
	{
		xchitiet=kc+175;
		ychitiet=ychitiet+(180/(sodinh+1));
		for(j=0;j<sodinh;j++)
		{
			xchitiet=xchitiet+(302/(sodinh+1));
			if(e[i][j]<0)
				sprintf(str,"%c",'~');
			else
				sprintf(str,"%d",e[i][j]);
			ngu_canh->TextOut(xchitiet+5,ychitiet+5,str);
			m_memDC.TextOut(xchitiet+5,ychitiet+5,str);
		}
	}
}

void cua_so::ve_ds_dinh()
{
	for(i=0;i<sodinh;i++)
	{
		sprintf(str,"%c",i+65);
		ve_dinh(dinh[i],RGB(0,128,0),RGB(0,0,0),RGB(255,255,255),str);
	}
}

void cua_so::wellcome()
{
	CBitmap a;
	CDC d,*c=GetDC();
	c=GetDC();
	//-------------
	i++;
	if(i>5)
		i=1;
	switch(i)
	{
		case 1:
			a.LoadBitmap(IDB_WELL1);
			d.CreateCompatibleDC(c);
			d.SelectObject(&a);
			c->BitBlt(kc+310,100,386,241,&d,0,0,SRCCOPY);
			m_memDC.BitBlt(kc+310,100,386,241,&d,0,0,SRCCOPY);
		//	KillTimer(113);
			break;
		case 2:
			a.LoadBitmap(IDB_WELL2);
			d.CreateCompatibleDC(c);
			d.SelectObject(&a);
			c->BitBlt(kc+311,98,386,241,&d,0,0,SRCCOPY);
			m_memDC.BitBlt(kc+311,98,386,241,&d,0,0,SRCCOPY);
			break;
		case 3:
			a.LoadBitmap(IDB_WELL3);
			d.CreateCompatibleDC(c);
			d.SelectObject(&a);
			c->BitBlt(kc+309,99,386,241,&d,0,0,SRCCOPY);
			m_memDC.BitBlt(kc+309,99,386,241,&d,0,0,SRCCOPY);
			break;
		case 4:
			a.LoadBitmap(IDB_WELL4);
			d.CreateCompatibleDC(c);
			d.SelectObject(&a);
			c->BitBlt(kc+310,99,386,241,&d,0,0,SRCCOPY);
			m_memDC.BitBlt(kc+310,99,386,241,&d,0,0,SRCCOPY);
			break;
		case 5:
			a.LoadBitmap(IDB_WELL5);
			d.CreateCompatibleDC(c);
			d.SelectObject(&a);
			c->BitBlt(kc+309,98,386,241,&d,0,0,SRCCOPY);
			m_memDC.BitBlt(kc+309,98,386,241,&d,0,0,SRCCOPY);
			break;
	}
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++HAM PHUC VU CHO MENU+++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void cua_so::OnThongtin()
{
	clrscr_main();
	SetTimer(113,1000,NULL);
	ID_DIALOG=3;
	hop_thoai d2(IDD_ABOUT,this);
	d2.DoModal();
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++HAM CAC THUAT TOAN+++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



//***********************
//*********DIJKSTRA******
//***********************

void cua_so::khoi_tao_dijkstra()
{
	dangxet=s;
    daxet[s]=1;
    //khoi tao
	xchitiet=kc+175;ychitiet=465;
	yghichu=465;
    for(i=0;i<sodinh;i++)
    {
        u[i].a =s;
        u[i].b = e[s][i];
    }
	clrscr_main();
	ve_do_thi();
	clrscr_ghichu();
	hoan_thanh=FALSE;
	delay=2000;
}


void cua_so::ve_duong_dijkstra()
{
	//Vong lap hien su thay doi
	if(u[t].b>0)
	{
//		clrscr_main();
		ve_do_thi();
		int k;
		k=t;
		while(k!=s)
		{
			ve_cung(k,u[k].a,RGB(0,0,255),RGB(255,0,0));
			sprintf(str,"%c",u[k].a+65);
			ve_dinh(dinh[u[k].a],RGB(0,0,255),RGB(0,0,0),RGB(255,255,255),str);
			sprintf(str,"%c",k+65);
			ve_dinh(dinh[k],RGB(0,0,255),RGB(0,0,0),RGB(255,255,255),str);			
			k=u[k].a;
		}
		sprintf(str,"%c",s+65);
		ve_dinh(dinh[s],RGB(0,128,0),RGB(0,0,0),RGB(255,255,255),str);
		sprintf(str,"%c",t+65);
		ve_dinh(dinh[t],RGB(255,0,0),RGB(0,0,0),RGB(255,255,255),str);
	}
}

void cua_so::dijkstra()
{
		///---------vong lap hien thong tin buoc----------
		font_chi_tiet(RGB(0,0,0));
		ychitiet=ychitiet+(180/(sodinh+1));
		xchitiet=kc+175;
		for(i=0;i<sodinh;i++)
		{
			xchitiet=xchitiet+(302/(sodinh+1));
			if(daxet[i]==1)
			{
				if(i==dangxet)
				{
					ngu_canh=GetDC();
					p.DeleteObject();
					p.CreatePen(PS_SOLID,1,RGB(0,0,0));
					b.DeleteObject();
					b.CreateSolidBrush(RGB(0,128,64));
					ngu_canh->SelectObject(p);
					m_memDC.SelectObject(p);
					ngu_canh->SelectObject(b);
					m_memDC.SelectObject(b);
					ngu_canh->SetBkMode(TRANSPARENT);
					m_memDC.SetBkMode(TRANSPARENT);
					ngu_canh->SetTextColor(RGB(255,255,255));
					m_memDC.SetTextColor(RGB(255,255,255));
					ngu_canh->Rectangle(xchitiet,ychitiet,xchitiet+(302/(sodinh+1)),ychitiet+(180/(sodinh+1)));
					m_memDC.Rectangle(xchitiet,ychitiet,xchitiet+(302/(sodinh+1)),ychitiet+(180/(sodinh+1)));
					sprintf(str,"%c,%d",u[i].a+65,u[i].b);
					font_chi_tiet(RGB(255,255,255));
					ngu_canh->TextOut(xchitiet+3,ychitiet+2,str);
					m_memDC.TextOut(xchitiet+3,ychitiet+2,str);
				}
				else
				{
					font_chi_tiet(RGB(0,0,0));
					ngu_canh->TextOut(xchitiet+10,ychitiet+2,"*");
					m_memDC.TextOut(xchitiet+10,ychitiet+2,"*");
				}
			}
			else
			{
				if(u[i].b==vocung)
				{
					sprintf(str,"%c,%c",u[i].a+65,'~');
				}
				else
				{
					sprintf(str,"%c,%d",u[i].a+65,u[i].b);
				}
				font_chi_tiet(RGB(0,0,0));
				ngu_canh->TextOut(xchitiet+3,ychitiet+2,str);
				m_memDC.TextOut(xchitiet+3,ychitiet+2,str);
			}
		}
		//thoat khi dinh co do dai ngan nhat hien tai la dinh dich
		if(dangxet==t)
		{
			thong_bao_dijkstra();
			return;
		}
        //vong lap dinh vi lai dinh
        i=0;
        vtmin=0;
        while(i<=sodinh && (daxet[i]==1 || u[i].b==vocung || u[i].b==0))
        {
            i++;
        }
		vtmin=i;
		if(vtmin>=sodinh)
		{
			thong_bao_dijkstra();
			return;
		}
		//------------dll--------------------------------------------------------
		typedef void (*FPointer)(int, int *, dt *, int*, int);
		FPointer f1;
		f1=(FPointer)GetProcAddress(h1,"tim_min");
		f1(sodinh, daxet, u, &vtmin, i);
		
        dangxet=vtmin;
        daxet[dangxet]=1;

		//vong lap ghan lai nhan
        for(i=0;i<sodinh;i++)
        {
            if((daxet[i]==0 && e[dangxet][i]>0) && (u[i].b>u[dangxet].b+e[dangxet][i] || u[i].b==vocung))
            {
				if(u[t].b>0)
				{
					int j;
					j=t;
					while(j!=s)
					{
						if(j==i)
						{
							KillTimer(110);
							nhap_nhay(i,u[i].a);
							nhap_nhay(i,dangxet);
							SetTimer(110,delay,NULL);
							break;
						}
						j=u[j].a;
					}
				}
				u[i].a=dangxet;
                u[i].b=u[dangxet].b+e[dangxet][i];
				ve_duong_dijkstra();
            }
        }
}


void cua_so::thong_bao_dijkstra()
{
	if(u[t].b<=0)
	{
		clrscr_ghichu();
		font_nho_i(RGB(255,0,0));
		sprintf(str,"Khong co duong di tu %c toi %c !",s+65,t+65);
		ngu_canh->TextOut(kc+545,yghichu,str);
		m_memDC.TextOut(kc+545,yghichu,str);
		KillTimer(110);
		play=FALSE;
		hoan_thanh=TRUE;
		//	return;
	}
	else
	{
		//Hien thong bao ket thuc
//		int j;
		clrscr_ghichu();
		font_nho_i(RGB(255,0,0));
		ngu_canh->TextOut(kc+545,yghichu,"Hoan thanh");	
		m_memDC.TextOut(kc+545,yghichu,"Hoan thanh");	
		//------------dll--------------------------------------------------------
		typedef BOOL (*FPointer)(int, int, int *, char *, dt *, int, int);
		FPointer fp;
		fp=(FPointer)GetProcAddress(h1,"dao_mang");
		fp(i, t, mangtg, str, u, s, sodinh);
		
		//--------------
		yghichu=yghichu+20;
		font_nho_i(RGB(0,0,0));
		ngu_canh->TextOut(kc+545,yghichu,"Duong di ngan nhat la: ");
		m_memDC.TextOut(kc+545,yghichu,"Duong di ngan nhat la: ");
		yghichu=yghichu+20;
		ngu_canh->TextOut(kc+545,yghichu,str);
		m_memDC.TextOut(kc+545,yghichu,str);
		yghichu=yghichu+20;
		sprintf(str,"Do dai duong di la: %d",u[t].b);
		ngu_canh->TextOut(kc+545,yghichu,str);
		m_memDC.TextOut(kc+545,yghichu,str);
	}
	//huy timer
	KillTimer(110);
	play=FALSE;
	hoan_thanh=TRUE;
}

//***********************
//**********PRIM*********
//***********************

void cua_so::khoi_tao_prim()
{
	dangxet=s;
    daxet[s]=1;
	xchitiet=kc+175;ychitiet=465;
	yghichu=465;
	//Khoi gan gia tri ban dau
    for(i=0;i<sodinh;i++)
    {
        u[i].a =s;
        u[i].b = e[s][i];
    }
	clrscr_main();
	clrscr_ghichu();
	ve_do_thi();
	hoan_thanh=false;
	delay=2000;
}



void cua_so::prim()
{
	ngu_canh=GetDC();
	if(dangxet<sodinh)
	{
		///---------vong lap hien thong tin buoc----------
		font_chi_tiet(RGB(0,0,0));
		ychitiet=ychitiet+(180/(sodinh+1));
		xchitiet=kc+175;
		for(i=0;i<sodinh;i++)
		{
			xchitiet=xchitiet+(302/(sodinh+1));
			if(daxet[i]==1)
			{
				if(i==dangxet)
				{
					ngu_canh=GetDC();
					p.DeleteObject();
					p.CreatePen(PS_SOLID,1,RGB(0,0,0));
					b.DeleteObject();
					b.CreateSolidBrush(RGB(0,128,64));
					ngu_canh->SelectObject(p);
					m_memDC.SelectObject(p);
					ngu_canh->SelectObject(b);
					m_memDC.SelectObject(b);
					ngu_canh->Rectangle(xchitiet,ychitiet,xchitiet+(302/(sodinh+1)),ychitiet+(180/(sodinh+1)));
					m_memDC.Rectangle(xchitiet,ychitiet,xchitiet+(302/(sodinh+1)),ychitiet+(180/(sodinh+1)));
					sprintf(str,"%c,%d",u[i].a+65,u[i].b);
					font_chi_tiet(RGB(255,255,255));
					ngu_canh->TextOut(xchitiet+3,ychitiet+2,str);
					m_memDC.TextOut(xchitiet+3,ychitiet+2,str);
				}
				else
				{
					font_chi_tiet(RGB(0,0,0));
					ngu_canh->TextOut(xchitiet+15,ychitiet+2,"*");
					m_memDC.TextOut(xchitiet+15,ychitiet+2,"*");
				}
			}
			else
			{
				font_chi_tiet(RGB(0,0,0));
				sprintf(str,"%c,%d",u[i].a+65,u[i].b);
				if(u[i].b==vocung)
				{
					sprintf(str,"%c,%c",u[i].a+65,'~');
				}
				else
				{
					sprintf(str,"%c,%d",u[i].a+65,u[i].b);
				}
				ngu_canh->TextOut(xchitiet+3,ychitiet+2,str);
				m_memDC.TextOut(xchitiet+3,ychitiet+2,str);
			}
		}
		//Hien thi cay khung
		for(i=sodinh-1;i>=0;i--)
		{
			if(i!=s && u[i].b>0)
			{
				ve_cung(u[i].a,i,RGB(0,0,255),RGB(255,0,0));
				sprintf(str,"%c",u[i].a+65);
				ve_dinh(dinh[u[i].a],RGB(0,0,255),RGB(0,0,0),RGB(255,255,255),str);
				sprintf(str,"%c",i+65);
				ve_dinh(dinh[i],RGB(0,0,255),RGB(0,0,0),RGB(255,255,255),str);
			}
		}
		///vong lap gan lai nhan
		for(i=0;i<sodinh;i++)
		{
			if((daxet[i]==0 && e[dangxet][i]>0) && (u[i].b>e[dangxet][i] || u[i].b==vocung))
			{
				if((daxet[i]==0 && e[dangxet][i]>0) && (u[i].b>e[dangxet][i]))
				{
					nhap_nhay(i,u[i].a);
					nhap_nhay(i,dangxet);
					ve_cung(u[i].a,i,RGB(0,0,0),RGB(0,0,0));
					ve_cung(i,dangxet,RGB(0,0,255),RGB(255,0,0));
					sprintf(str,"%c",u[i].a+65);
					ve_dinh(dinh[u[i].a],RGB(255,255,255),RGB(0,0,0),RGB(255,255,255),str);
					sprintf(str,"%c",i+65);
					ve_dinh(dinh[i],RGB(0,0,255),RGB(0,0,0),RGB(255,255,255),str);
					sprintf(str,"%c",dangxet+65);
					ve_dinh(dinh[dangxet],RGB(0,0,255),RGB(0,0,0),RGB(255,255,255),str);
				}
				u[i].a=dangxet;
				u[i].b=e[dangxet][i];
			}
		}
		///vong lap dinh vi lai dinh
		//------------dll--------------------------------------------------------
		typedef int (*FPointer)(int, int, int, int *, dt *);
		FPointer fvtmin;
		fvtmin=(FPointer)GetProcAddress(h1,"min_prim");
	//---------------
		dangxet=fvtmin(i, vtmin, sodinh, daxet, u);
		daxet[dangxet]=1;
	}
	else
	{
		thong_bao_prim();
		return;
	}
}


void cua_so::thong_bao_prim()
{
	//Hien thong bao ket thuc
	clrscr_ghichu();
	font_nho_i(RGB(255,0,0));
	
	char stg[10];
	str[0]='\0';
	for(i=sodinh-1;i>=0;i--)
	{
		if(i!=s)
		{
			sprintf(stg,"%c%c",u[i].a+65,i+65);
			strcat(str,stg);
			if(i>0)
				strcat(str,", ");
		}
	}
	strcat(str,".");
	yghichu=yghichu+20;
	font_nho_i(RGB(255,0,0));
	ngu_canh->TextOut(kc+545,yghichu,"Cay khung be nhat la: ");
	m_memDC.TextOut(kc+545,yghichu,"Cay khung be nhat la: ");
	yghichu=yghichu+20;
	font_nho_i(RGB(0,0,0));
	ngu_canh->TextOut(kc+545,yghichu,str);
	m_memDC.TextOut(kc+545,yghichu,str);
	//huy timer
	KillTimer(111);
	play=FALSE;
	CBitmap a;
	CDC d,*c=GetDC();
	c=GetDC();
	a.LoadBitmap(IDB_PLAY1);
	d.CreateCompatibleDC(c);
	d.SelectObject(&a);
	c->BitBlt(kc+471,372,66,40,&d,0,0,SRCCOPY);
	d.DeleteDC();
	c->DeleteDC();
	a.DeleteObject();
	hoan_thanh=TRUE;
}


//***********************
//**********DFS*********
//***********************


BOOL cua_so::dfs(int u)
{
	int i,dem;
	dem=0;
	for(i=0;i<sodinh;i++)
	{
		if(daxet[i]==1)
			dem++;
	}
	font_nho_i(RGB(0,0,0));
	if(dem<sodinh-1)
		sprintf(str," %c ->",u+65);
	else
		sprintf(str," %c.",u+65);
	ngu_canh->TextOut(xghichu,yghichu,str);
	m_memDC.TextOut(xghichu,yghichu,str);
	xghichu+=40;
	if(xghichu>kc+750)
	{
		xghichu=kc+545;
		yghichu+=20;
	}
    if(daxet[u]==0)
    {
        daxet[u]=1;
    }
    for(i=0;i<sodinh;i++)
    {
        if(e[u][i]>0 && daxet[i]==0)
        {
            truoc[i]=u;
		}
	}
	for(i=0;i<sodinh;i++)
    {
        if(e[u][i]>0 && daxet[i]==0)
        {
            nhap_nhay(u,i);
			sprintf(str,"%c",u+65);
			ve_dinh(dinh[u],RGB(0,128,0),RGB(0,0,0),RGB(255,255,255),str);
            dfs(i);
		}
	}
			
    if(i==sodinh)
	{
		dem=0;
		for(i=0;i<sodinh;i++)
		{
			if(daxet[i]==1)
				dem++;
		}
		if(dem==sodinh)
		{
			sprintf(str,"%c",u+65);
			ve_dinh(dinh[u],RGB(0,128,0),RGB(0,0,0),RGB(255,255,255),str);
			play=FALSE;
			CBitmap a;
			CDC d,*c=GetDC();
			c=GetDC();
			a.LoadBitmap(IDB_PLAY1);
			d.CreateCompatibleDC(c);
			d.SelectObject(&a);
			c->BitBlt(kc+471,372,66,40,&d,0,0,SRCCOPY);
			a.DeleteObject();
			d.DeleteDC();
			c->DeleteDC();
			return TRUE;
		}
		else
		{
			sprintf(str,"%c",u+65);
			ve_dinh(dinh[u],RGB(0,0,255),RGB(0,0,0),RGB(255,255,255),str);
			Sleep(delay/4);
			sprintf(str,"%c",u+65);
			ve_dinh(dinh[u],RGB(255,255,255),RGB(0,0,0),RGB(0,0,0),str);
			Sleep(delay/4);
			sprintf(str,"%c",u+65);
			ve_dinh(dinh[u],RGB(0,0,255),RGB(0,0,0),RGB(255,255,255),str);
			Sleep(delay/4);
			sprintf(str,"%c",u+65);
			ve_dinh(dinh[u],RGB(0,128,0),RGB(0,0,0),RGB(255,255,255),str); 

			///////////
			sprintf(str,"%c",truoc[u]+65);
			ve_dinh(dinh[truoc[u]],RGB(0,0,255),RGB(0,0,0),RGB(255,255,255),str);
			Sleep(delay/4);
			sprintf(str,"%c",truoc[u]+65);
			ve_dinh(dinh[truoc[u]],RGB(255,255,255),RGB(0,0,0),RGB(0,0,0),str);
			Sleep(delay/4);
			sprintf(str,"%c",truoc[u]+65);
			ve_dinh(dinh[truoc[u]],RGB(0,0,255),RGB(0,0,0),RGB(255,255,255),str);
			Sleep(delay/4);

			dem=0;
			if(truoc[u]==s)
			{
				for(i=0;i<sodinh;i++)
				{
					if(e[s][i]>0 && daxet[i]==0)
					{
						dem++;
						break;
					}
				}
				if(dem==0)
				{
					sprintf(str,"%c",s+65);
					ve_dinh(dinh[s],RGB(0,128,0),RGB(0,0,0),RGB(255,255,255),str);
					play=FALSE;
					return FALSE;
				}
				else
					dfs(truoc[u]);
			}
		}
	}
	play=FALSE;
	return TRUE;
}


//***********************
//**********BFS*********
//***********************


void cua_so::bfs(int s)
{
    int i,v,j;
    daxet[s]=1;
    for(i=0;i<sodinh;i++)
    {
        mangtg[i]=-1;
    }
    mangtg[0]=s;
    while(mangtg[0]!=-1)
    {
        j=0;
        while(j<sodinh && mangtg[j]!=-1)
        {
            j++;
        }
        v=mangtg[j-1];
		font_nho_i(RGB(0,0,0));
		if(mangtg[1]==-1 && mangtg[0]!=s)
			sprintf(str," %c.",v+65);
		else
			sprintf(str," %c ->",v+65);
		ngu_canh->TextOut(xghichu,yghichu,str);
		m_memDC.TextOut(xghichu,yghichu,str);
		xghichu+=40;
		if(xghichu>kc+750)
		{
			xghichu=kc+545;
			yghichu+=20;
		}
		sprintf(str,"%c",v+65);
		ve_dinh(dinh[v],RGB(255,0,0),RGB(0,0,0),RGB(255,255,255),str);
		Sleep(delay);

        mangtg[j-1]=-1;
        for(i=0;i<sodinh;i++)
        {
            if(e[v][i]>0 && daxet[i]==0)
            {
                j=0;
                while(mangtg[j]!=-1 && j<sodinh)
                {
                    j++;
                }
                while(j>0)
                {
                    mangtg[j]=mangtg[j-1];
                    j--;
                }
                mangtg[0]=i;
                daxet[i]=1;
				nhap_nhay(v,i);
				sprintf(str,"%c",i+65);
				ve_dinh(dinh[i],RGB(255,255,255),RGB(0,0,0),RGB(0,0,0),str);
            }
			sprintf(str,"%c",v+65);
			ve_dinh(dinh[v],RGB(0,128,0),RGB(0,0,0),RGB(255,255,255),str);
        }
		if(mangtg[0]==-1)
		{
			int dem=0;
			for(i=0;i<sodinh;i++)
			{
				if(daxet[i]==1)
				{
					dem++;
				}
			}
			if(dem<sodinh)
			{
				xghichu=kc+545;
				yghichu+=20;
				font_nho_i(RGB(255,0,0));
				ngu_canh->TextOut(xghichu,yghichu,"Do thi khong lien thong !");
				m_memDC.TextOut(xghichu,yghichu,"Do thi khong lien thong !");
				break;
			}
		}
    }
	play=FALSE;
	CBitmap a;
	CDC d,*c=GetDC();
	c=GetDC();
	a.LoadBitmap(IDB_PLAY1);
	d.CreateCompatibleDC(c);
	d.SelectObject(&a);
	c->BitBlt(kc+471,372,66,40,&d,0,0,SRCCOPY);
	a.DeleteObject();
	d.DeleteDC();
	c->DeleteDC();
}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++CAC HAM CUA DIALOG+++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL hop_thoai::OnInitDialog()
{
	
	CDialog::OnInitDialog();
	int i;
	if(ID_DIALOG==0)
	{
		cbo=(CComboBox*)GetDlgItem(ID_XUATPHAT);
		for(i=0;i<sodinh;i++)
		{
			sprintf(str,"%c",i+65);
			cbo->AddString(str);
		}
		cbo->SetWindowText("A");
		cbo=(CComboBox*)GetDlgItem(ID_KETTHUC);
		for(i=0;i<sodinh;i++)
		{
			sprintf(str,"%c",i+65);
			cbo->AddString(str);
		}
		cbo->SetWindowText("B");
	}
	else
		if(ID_DIALOG==1)
		{
			cbo=(CComboBox*)GetDlgItem(IDC_DINHXUATPHAT);
			for(i=0;i<sodinh;i++)
			{
				sprintf(str,"%c",i+65);
				cbo->AddString(str);
			}
			cbo->SetWindowText("A");
		}
	else
		if(ID_DIALOG==2)
		{
			ed=(CEdit*)GetDlgItem(ID_TRONGSO);
			ed->SetWindowText("5");
			ed->SetFocus();
		}
	else
		if(ID_DIALOG==3)
		{

		}
	return true;
}

void hop_thoai::OnOK()
{
	CDialog::OnOK();
	chon_sai=FALSE;
	if(ID_DIALOG==0)
	{
		int i;
		char stg[5];
		cbo=(CComboBox*)GetDlgItem(ID_XUATPHAT);
		cbo->GetWindowText(str,10);
		for(i=0;i<sodinh;i++)
		{
			stg[0]=i+65;
			stg[1]='\0';
			if(strcmp(str,stg)==0)
			{
				s=i;
				break;
			}
		}
		if(i==sodinh)
		{
			AfxMessageBox("Ban chon sai dinh",MB_ICONWARNING);
			chon_sai=TRUE;
			return;
		}
		cbo=(CComboBox*)GetDlgItem(ID_KETTHUC);
		cbo->GetWindowText(str,10);
		for(i=0;i<sodinh;i++)
		{
			stg[0]=i+65;
			stg[1]='\0';
			if(strcmp(str,stg)==0)
			{
				t=i;
				break;
			}
		}
		if(i==sodinh)
		{
			AfxMessageBox("Ban chon sai dinh",MB_ICONWARNING);
			chon_sai=TRUE;
			return;
		}
		if(t==s)
		{
			AfxMessageBox("Dinh xuat phat va dinh dich phai khac nhau",MB_ICONWARNING);
			chon_sai=TRUE;
			return;
		}
		chon_sai=FALSE;
	}
	else
		if(ID_DIALOG==1)
		{
			int i;
			char stg[5];
			cbo=(CComboBox*)GetDlgItem(IDC_DINHXUATPHAT);
			cbo->GetWindowText(str,10);
			for(i=0;i<sodinh;i++)
			{
				stg[0]=i+65;
				stg[1]='\0';
				if(strcmp(str,stg)==0)
				{
					s=i;
					break;
				}
			}
			if(i==sodinh)
			{
				AfxMessageBox("Ban chon sai dinh",MB_ICONWARNING);
				chon_sai=TRUE;
			}
			else
				chon_sai=FALSE;
		}
	else
		if(ID_DIALOG==2)
		{
			ed=(CEdit*)GetDlgItem(ID_TRONGSO);
			ed->GetWindowText(str,20);
			trongso=atoi(str);
			if(trongso<1 || trongso>100)
			{
				AfxMessageBox("Trong so nam trong khoang 1 den 99.",MB_ICONWARNING);
				chon_sai=TRUE;
			}
			else
				chon_sai=FALSE;
		}
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++CAC HAM LIEN QUAN DEN DU LIEU++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++




void cua_so::open(char *name)
{
	int i,j;
		if(f.Open(name,CFile::modeRead))
		{
			f.Read(&sodinh,sizeof(int));
			dinh=(CPoint *) realloc(dinh,sodinh*sizeof(CPoint));
			e=(int **) realloc(e,sodinh*sizeof(int*));
			e[0]=(int *) realloc(e[0],sodinh*sizeof(int));
			for(i=1;i<sodinh;i++)
			{
				e[i]=(int *) malloc(sodinh*sizeof(int));	
			}
			for(i=0;i<sodinh;i++)
			{
				f.Read(&dinh[i],1*sizeof(CPoint));
			}
			for(i=0;i<sodinh;i++)
				for(j=0;j<sodinh;j++)
				{
					f.Read(&e[i][j],1*sizeof(int));
				}
				f.Close();
		}

	u=(dt *) realloc(u,sodinh*sizeof(dt));
	daxet = (int *) realloc(daxet,sodinh*sizeof(int));
	truoc = (int *) realloc(truoc,sodinh*sizeof(int));
	mangtg = (int *) realloc(mangtg,sodinh*sizeof(int));
	for(i=0;i<sodinh;i++)
	{
		daxet[i]=0;
		truoc[i]=0;
		mangtg[i]=0;
		u[i].a=0;
		u[i].b=0;
	}
	
}

void cua_so::save(char *name)
{
	int i,j;
	//	CFileDialog fd(FALSE);
	//	if(fd.DoModal()==IDOK)
	//	{
	if(f.Open(name,CFile::modeCreate|CFile::modeWrite))
	{
		f.Write(&sodinh,sizeof(int));
		for(i=0;i<sodinh;i++)
		{
			f.Write(&dinh[i],sizeof(CPoint));
		}
		
		for(i=0;i<sodinh;i++)
			for(j=0;j<sodinh;j++)
			{
				f.Write(&e[i][j],sizeof(int));
			}
			f.Close();
	}
	char stg[30];
	sprintf(stg,"Luu file: ");
	strcat(stg,str);
	strcat(stg," thanh cong !");
	AfxMessageBox(stg);
	//	}
//	luu=FALSE;
}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++CHUC NANG TAO DO THI+++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


int cua_so::trong_dinh(CPoint diem, int boqua)
{
	if(sodinh==0)
		return-1;
	int i;
	for(i=0;i<sodinh;i++)
	{
		if(i!= boqua && sqrt(pow(diem.x-dinh[i].x,2)+pow(diem.y-dinh[i].y,2))<=40)
			return i;
	}
	return-1;
}

void cua_so::xoa_dinh(int vt)
{
	int i;
	if(vt==sodinh-1)
	{
		dinh = (CPoint *) realloc(dinh,(sodinh-1)*sizeof(CPoint));
		sodinh--;
		return;
	}
	for(i=vt;i<sodinh-1;i++)
	{
		dinh[i]=dinh[i+1];
	}
	dinh = (CPoint *) realloc(dinh,(sodinh-1)*sizeof(CPoint));
	sodinh--;
	return;
}


int cua_so::last_file()
{
	i=0;
	CFileException e;
	do 
	{
		sprintf(str,"DATA/DT%d.DAT",i);
		if( !f.Open( str, CFile::modeRead , &e ) )
		{
			return i;
		}
		f.Close();
		i++;
	} while (1);
}


void cua_so::OnTao()
{
	KillTimer(113);
	if(hoan_thanh==TRUE && play==FALSE)
	{
		menu_tao=TRUE;
		RECT r;
		ngu_canh=GetDC();
		GetClientRect(&r);
		kc=(r.right-1002)/2;
		bit.LoadBitmap(IDB_DAODIEN2);
		cdctg.CreateCompatibleDC(ngu_canh);
		cdctg.SelectObject(&bit);
		ngu_canh->BitBlt(kc,0,1002,684,&cdctg,0,0,SRCCOPY);  
		m_memDC.BitBlt(kc,0,1002,684,&cdctg,0,0,SRCCOPY); 
		//	ngu_canh->DeleteDC();
		cdctg.DeleteDC();
		bit.DeleteObject();
		//---------------
		sodinh=0;
		dinh=(CPoint *) realloc(dinh,1*sizeof(CPoint));
		e=(int **) realloc(e,1*sizeof(int*));
		e[0]=(int *) malloc(1*sizeof(int));
	} 
}
void cua_so::OnChon()
{
	KillTimer(113);
	int i,j;
	CFileDialog fd(TRUE);
	if(fd.DoModal()==IDOK)
	{
		if(f.Open(fd.GetPathName(),CFile::modeRead))
		{
			f.Read(&sodinh,sizeof(int));
			dinh=(CPoint *) realloc(dinh,sodinh*sizeof(CPoint));
			e=(int **) realloc(e,sodinh*sizeof(int*));
			e[0]=(int *) realloc(e[0],sodinh*sizeof(int));
			for(i=1;i<sodinh;i++)
			{
				e[i]=(int *) malloc(sodinh*sizeof(int));	
			}
			for(i=0;i<sodinh;i++)
			{
				f.Read(&dinh[i],1*sizeof(CPoint));
			}
			for(i=0;i<sodinh;i++)
				for(j=0;j<sodinh;j++)
				{
					f.Read(&e[i][j],1*sizeof(int));
				}
				f.Close();
		}
	}
	
	u=(dt *) realloc(u,sodinh*sizeof(dt));
	daxet = (int *) realloc(daxet,sodinh*sizeof(int));
	truoc = (int *) realloc(truoc,sodinh*sizeof(int));
	mangtg = (int *) realloc(mangtg,sodinh*sizeof(int));
	for(i=0;i<sodinh;i++)
	{
		daxet[i]=0;
		truoc[i]=0;
		mangtg[i]=0;
		u[i].a=0;
		u[i].b=0;
	}
	clrscr_main();
	ve_do_thi();
	bchon_dothi=TRUE;
	menu_tao=FALSE;
//	so_dinh_cu=sodinh;
}
void cua_so::OnThoat()
{
	if(MessageBox("Ban chac chan muon thoat ?","Thong bao",MB_YESNO|MB_ICONQUESTION)==IDYES)
		DestroyWindow();
}