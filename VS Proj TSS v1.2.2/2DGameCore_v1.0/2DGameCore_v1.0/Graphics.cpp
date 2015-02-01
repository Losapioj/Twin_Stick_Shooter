#include "LibDec.h"

/////////////////////////////////////////////////
// Needed library files
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

/////////////////////////////////////////////////
cGraphics g_graphics;

/////////////////////////////////////////////////
cGraphics::cGraphics()
: m_d3d (NULL), m_dev (NULL), m_backBuffer(NULL),
  m_sprite (NULL)
{}

/////////////////////////////////////////////////
cGraphics::~cGraphics()
{ 
	if (DEBUG_MODE) 
		debug << "GRAPHICS DESTRUCTOR" << endl;
	while(!m_fonts.empty())
	{
		FONT_ITER it = m_fonts.begin();
		if(it->second != NULL)
			it->second->Release();
		m_fonts.erase(it);
	}
    
	if (DEBUG_MODE) 
		debug << "1" << endl;
	while(!m_srfImages.empty())
	{
		SRF_ITER it = m_srfImages.begin();
		if(it->second != NULL)
			it->second->Release();
		m_srfImages.erase(it);
	}
  
	if (DEBUG_MODE) 
		debug << "2" << endl;
	while(!m_txrImages.empty())
	{
		TXR_ITER it = m_txrImages.begin();
		if(it->second != NULL)
			it->second->Release();
		m_txrImages.erase(it);
	}

	if (DEBUG_MODE) 
		debug << "3" << endl;
	SAFE_RELEASE(m_sprite);
	SAFE_RELEASE(m_backBuffer);
	SAFE_RELEASE(m_dev);
	SAFE_RELEASE(m_d3d);
	if (DEBUG_MODE) 
		debug << "GRAPHICS DESTRUCTOR" << endl;
}

/////////////////////////////////////////////////
bool cGraphics::Init(ifstream& fin)
{
	// Create and verify Direct3D interface object
	m_d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if(m_d3d == NULL)
	{
		ErrorMsg("ERROR: Direct3D Initialization ");
		return false;
	}

	// Create & load Presentation Parameter struct
	D3DPRESENT_PARAMETERS pp;
	ZeroMemory(&pp, sizeof(pp));

	// Assign attributes that don't 
	// depend on window state
	pp.Windowed = !g_app.IsFullscreen ();
	pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	if (pp.Windowed)
		// if windowed mode, use desktop pixel format
		pp.BackBufferFormat = D3DFMT_UNKNOWN;
	else
	{
		// if full screen mode...
		pp.BackBufferFormat = D3DFMT_X8R8G8B8;
		pp.BackBufferCount = 1;
		pp.BackBufferWidth = g_app.GetWindowWidth();
		pp.BackBufferHeight = g_app.GetWindowHeight ();
		pp.hDeviceWindow = g_app.GetWindowHandle();
	}
 
	HWND hwnd = g_app.GetWindowHandle();
	// Create & verify graphic device interface object
	HRESULT hr = m_d3d->CreateDevice(
		D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, 
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, &pp, &m_dev);
	if(hr != D3D_OK)
	{
		debug << "ERROR: Graphics Initialization" 
			  << endl;
		return false;
	}

	m_dev->GetBackBuffer(0, 0,
		D3DBACKBUFFER_TYPE_MONO, &m_backBuffer);

	D3DXCreateSprite(m_dev, &m_sprite);

	LoadFonts(fin);
	LoadSurfaces(fin);
	LoadTextures(fin);

  return true;
}

/////////////////////////////////////////////////
bool cGraphics::LoadFonts(ifstream& fin)
{
	D3DXFONT_DESC fontDesc;
	string id;
	LPD3DXFONT font = NULL;
	fin >> id;
	if(id != "[BeginFontList]")
	{
		debug << "Error reading data file" << endl;
		return false;
	}
	fin >> id;
	while(id != "[EndFontList]")
	{    
		ZeroMemory(&fontDesc, sizeof (fontDesc));
		fin.ignore();
		fin.getline((char*)fontDesc.FaceName, 31);
		fin >> fontDesc.Height;
		D3DXCreateFontIndirect(m_dev, 
			&fontDesc, &font);
    
		m_fonts[id] = font;
		fin >> id;
	}
	return true;
}

/////////////////////////////////////////////////
bool cGraphics::LoadSurfaces(ifstream& fin)
{
	string id, file;
	D3DXIMAGE_INFO imageInfo;
	LPDIRECT3DSURFACE9 image = NULL;

	fin >> id;
	if(id != "[BeginSurfaceList]")
	{
		debug << "Error reading data file" << endl;
		return false;
	}

	fin >> id;
	while(id != "[EndSurfaceList]")
	{
		fin.ignore();
		getline(fin, file);

		D3DXGetImageInfoFromFile (file.c_str(), &imageInfo);
    
		m_dev->CreateOffscreenPlainSurface(
			imageInfo.Width, imageInfo.Height, 
			D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, 
			&image, NULL);

		D3DXLoadSurfaceFromFile (image, NULL, 
			NULL, file.c_str (), NULL, D3DX_DEFAULT, 
			D3DCOLOR_XRGB (0, 0, 0), NULL);

		m_srfImages[id] = image;
		fin >> id;
	}
	return true;
}

/////////////////////////////////////////////////
bool cGraphics::LoadTextures(ifstream& fin)
{
	string id, file;
	D3DXIMAGE_INFO imageInfo;
	LPDIRECT3DTEXTURE9 image = NULL;

	fin >> id;
	if(id != "[BeginTextureList]")
	{
		debug << "Error reading data file" << endl;
		return false;
	}

	fin >> id;
	while(id != "[EndTextureList]")
	{
		fin.ignore();
		getline(fin, file);

		D3DXGetImageInfoFromFile (file.c_str (), &imageInfo);

		D3DXCreateTextureFromFileEx(
			m_dev, file.c_str(),imageInfo.Width, 
			imageInfo.Height, 1, D3DPOOL_DEFAULT, 
			D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, 
			D3DX_DEFAULT, D3DX_DEFAULT, 0xffff00ff,
			&imageInfo, NULL, &image);

		m_txrImages[id] = image;
		fin >> id;
	}
	return true;
}

/////////////////////////////////////////////////
bool cGraphics::Lock ()
{ 
	if (m_dev->BeginScene () != D3D_OK)
		return  false;

	if (m_sprite->Begin(D3DXSPRITE_ALPHABLEND) != D3D_OK)
		return false;

	return true;
}

/////////////////////////////////////////////////
bool cGraphics::Unlock ()
{
	if(m_sprite->End() != D3D_OK)
		return false;

	if(m_dev->EndScene () != D3D_OK)
		return false;

	return true;
}

/////////////////////////////////////////////////
void cGraphics::ClearScreen(int r, int g, int b)
{
	m_dev->Clear (0, NULL, D3DCLEAR_TARGET, 
		D3DCOLOR_XRGB(r, g, b), 1.0f, 0);
}

/////////////////////////////////////////////////
void cGraphics::FillRect(RECT* rc, DWORD color)
{
	Lock();
	HRESULT hr = m_dev->ColorFill(m_backBuffer, rc, color);
	if(hr != D3D_OK)
		ErrorMsg("FillRect");
	Unlock();
}

/////////////////////////////////////////////////
bool cGraphics::DrawText (const string& id, 
						  const string& s, RECT& rc, int format, 
						  int r, int g, int b)
{
	FONT_ITER it = m_fonts.find(id);
	if(it == m_fonts.end())
		return false;

	HRESULT hr = m_fonts[id]->DrawText (
		NULL, s.c_str (), -1, &rc, format, 
		D3DCOLOR_XRGB(r, g, b));
	return hr == D3D_OK;
}

/////////////////////////////////////////////////
bool cGraphics::DrawSurface (const string& id, RECT* src, RECT* dest)  
{
	SRF_ITER it = m_srfImages.find (id);
	if (it == m_srfImages.end ())
	{
		debug << "Image ID not found" << endl;
		return false;
	}

	m_dev->StretchRect(m_srfImages[id], src, 
		m_backBuffer, dest, D3DTEXF_NONE);

	return true;
}

/////////////////////////////////////////////////
bool cGraphics::DrawTexture (const string& id, RECT* src, sVector2d* pos, 
							 sVector2d* center, sVector2d* scale, 
							 float rotate, DWORD color)  
{
	D3DXMATRIX oldMat, newMat;

	D3DXMatrixTransformation2D(&newMat, 
		(D3DXVECTOR2*)center, 0.0f, 
		(D3DXVECTOR2*)scale, (D3DXVECTOR2*)center, 
		rotate, (D3DXVECTOR2*)pos);

	m_sprite->SetTransform(&newMat);

	m_sprite->Draw(m_txrImages[id],src, NULL, NULL, color);

	D3DXMatrixIdentity(&newMat);
	m_sprite->SetTransform(&newMat);

	return true;
}

/////////////////////////////////////////////////
void cGraphics::Display()
{
	m_dev->Present (NULL, NULL, NULL, NULL);
}