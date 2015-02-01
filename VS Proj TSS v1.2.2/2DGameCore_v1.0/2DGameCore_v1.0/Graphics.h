#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__


///////////////////////////////////////////
// Represents all graphical functionality//
///////////////////////////////////////////
class cGraphics
{
private:
	typedef map <string,LPD3DXFONT> FONT_LIST;
	typedef FONT_LIST::iterator FONT_ITER;

	typedef map<string, LPDIRECT3DSURFACE9> SRF_LIST;
	typedef SRF_LIST::iterator SRF_ITER;
 
	typedef map<string, LPDIRECT3DTEXTURE9> TXR_LIST;
	typedef TXR_LIST::iterator TXR_ITER;

	LPDIRECT3D9 m_d3d;
	LPDIRECT3DDEVICE9 m_dev;
	LPDIRECT3DSURFACE9 m_backBuffer;
	LPD3DXSPRITE m_sprite;

	FONT_LIST m_fonts;
	SRF_LIST m_srfImages;
	TXR_LIST m_txrImages;

	bool LoadFonts(ifstream& fin);
	bool LoadSurfaces(ifstream& fin);
	bool LoadTextures(ifstream& fin);

public:
	cGraphics();
	~cGraphics();
	bool Init(ifstream& fin);

	bool Lock();
	bool Unlock();
  
	void FillRect (RECT* rc, DWORD color);
	void ClearScreen(int r, int g, int b);
	void Display();

	bool DrawText(const string& id, 
				  const string& s, RECT& rc, int format, 
				  int r, int g, int b);
	bool DrawSurface(const string& id, RECT* src, RECT* dest);
  
	bool DrawTexture(const string& id, RECT* src, sVector2d* pos, 
					 sVector2d* center, sVector2d* scale, 
					 float rotate, DWORD color);
};

/////////////////////////////////////////////////
#endif // __GRAPHICS_H__