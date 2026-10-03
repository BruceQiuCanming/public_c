#include "stdafx.h"
#include "Bmp.h"
#include "common.h"

BOOL WriteBmp(CString strFile,BYTE *vtData,DWORD dataLen,const SIZE &sizeImg)   
   {     
     
       BITMAPINFOHEADER bmInfoHeader = {0};  
       bmInfoHeader.biSize = sizeof(BITMAPINFOHEADER);  
       bmInfoHeader.biWidth = sizeImg.cx;  
       bmInfoHeader.biHeight = sizeImg.cy;  
       bmInfoHeader.biPlanes = 1;  
       bmInfoHeader.biBitCount = 24;  
     
       //Bimap file header in order to write bmp file  
       BITMAPFILEHEADER bmFileHeader = {0};  
       bmFileHeader.bfType = 0x4d42;  //bmp    
       bmFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);  
       bmFileHeader.bfSize = bmFileHeader.bfOffBits + ((bmInfoHeader.biWidth * bmInfoHeader.biHeight) * 3); ///3=(24 / 8)  

    
       HANDLE hFile = CreateFile(strFile,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);  
       if(hFile == INVALID_HANDLE_VALUE)  
       {  
           return FALSE;  
       }  
     
       DWORD dwWrite = 0;  
       WriteFile(hFile,&bmFileHeader,sizeof(BITMAPFILEHEADER),&dwWrite,NULL);  
       WriteFile(hFile,&bmInfoHeader, sizeof(BITMAPINFOHEADER),&dwWrite,NULL); 

       if(WriteFile(hFile,&vtData[0], dataLen , &dwWrite,NULL) == false)
	   {
		   AfxMessageBox(_T("保存文件失败!"));
	   }

	   if(dwWrite == 0)
	   {
		    AfxMessageBox(_T("保存文件失败!"));
	   }
    
     
       CloseHandle(hFile);  
		
       return TRUE;  
   }   
     
     
   BOOL WriteBmp(CString strFile,HDC hdc)  
   {  
       int iWidth = GetDeviceCaps(hdc,HORZRES);  
       int iHeight = GetDeviceCaps(hdc,VERTRES);  
       RECT rcDC = {0,0,iWidth,iHeight};  
     
       return WriteBmp(strFile,hdc,rcDC);    
   }  
     
   BOOL WriteBmp(CString strFile,HDC hdc,CRect rcDC)  
   {  
       BOOL bRes = FALSE;  
       BITMAPINFO bmpInfo = {0};  
       BYTE *pData = NULL;  
       SIZE sizeImg = {0};  
       HBITMAP hBmp = NULL;  
       BYTE * vtData;  
       HGDIOBJ hOldObj = NULL;  
       HDC hdcMem = NULL;  
     
       //Initilaize the bitmap information   
       bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);  
       bmpInfo.bmiHeader.biWidth = rcDC.right - rcDC.left;  
       bmpInfo.bmiHeader.biHeight = rcDC.bottom - rcDC.top;  
       bmpInfo.bmiHeader.biPlanes = 1;  
       bmpInfo.bmiHeader.biBitCount = 24; 

 
       //Create the compatible DC to get the data  
       hdcMem = CreateCompatibleDC(hdc);  
       if(hdcMem == NULL)  
       {  
           goto EXIT;  
       }  

       //Get the data from the memory DC     
       hBmp = CreateDIBSection(hdcMem,&bmpInfo,DIB_RGB_COLORS,reinterpret_cast<VOID **>(&pData),NULL,0);  
       if(hBmp == NULL)  
       {  
           goto EXIT;  
       }  

       hOldObj = SelectObject(hdcMem, hBmp);  
  
       //Draw to the memory DC  
       sizeImg.cx = bmpInfo.bmiHeader.biWidth;  
       sizeImg.cy = bmpInfo.bmiHeader.biHeight;  
       StretchBlt(hdcMem,  
                   0,  
                   0,  
                   sizeImg.cx,  
                   sizeImg.cy,  
                   hdc,  
                    rcDC.left,  
                    rcDC.top,  
                    rcDC.right - rcDC.left + 1,  
                    rcDC.bottom - rcDC.top + 1,  
                    SRCCOPY);  
          
      
	    vtData = (BYTE*)GlobalAlloc(GMEM_FIXED, sizeImg.cx * sizeImg.cy * 3);

        memcpy(&vtData[0],pData,sizeImg.cx * sizeImg.cy * 3);  


		bRes = WriteBmp(strFile,vtData,sizeImg.cx * sizeImg.cy * 3,sizeImg);  
      
        SelectObject(hdcMem, hOldObj);  
          
		GlobalFree((HANDLE)vtData);
      
    EXIT:  
        if(hBmp != NULL)  
        {  
            DeleteObject(hBmp);  
        }  
      
        if(hdcMem != NULL)  
        {  
            DeleteDC(hdcMem);  
        }  
      
        return bRes;  
    }  
   
   
 


void DrawTempFlat(int BoxNr,CDC *dc,SWITCH_TEST_RESULT  *pSwitchTestResult ,SWITCH_CONFIG_PARA *pSwitchConfigPara ,CRect ChartRect,CTime BeginTestTime, bool IsOpenTemp,BOX_TYPE BoxType,BOOL FontDirection)
{
	
	
	int total[200],sum_total = 0;
	CONTROL_TEMP_RANGE temp_range;

	float max_temp = -999;
	float min_temp =  999;

	
	CheckHeatOrCoolMode(BoxNr,*pSwitchConfigPara,&temp_range,BoxType);

	if((IsOpenTemp == false) && (temp_range.close_temp_min == temp_range.close_temp_max))
	{
	//	AfxMessageBox(_T("没有测试复位温度"));
		return;
	}
	

	int x_min,x_max;
	if(IsOpenTemp)
	{
		x_min = (int)(temp_range.open_temp_min - 1);
		x_max = (int)(temp_range.open_temp_max + 1);
	}
	else
	{
		x_min = (int)(temp_range.close_temp_min - 1);
		x_max = (int)(temp_range.close_temp_max + 1);
	}

	memset(total,0,sizeof(total));

	for(int i = 0; i < G_iMaxLedNr[BoxNr]; i++)
	{
		if(pSwitchTestResult[i].IsUsed)
		{
			sum_total ++;
			float temp;
			if(IsOpenTemp)
			{
				if(pSwitchTestResult[i].IsOpenned)
				{
					temp = (pSwitchTestResult[i].OpenTemp);
					if(temp > max_temp)
					{
						max_temp = temp;
					}

					if(temp < min_temp)
					{
						min_temp = temp;
					}
					if(temp < temp_range.open_temp_min)
					{
						temp = x_min;
					}
					else if(temp > temp_range.open_temp_max)
					{
						temp = x_max;
					}
				}
				else
				{
					temp = x_max ;
				}
			}
			else
			{
				if(pSwitchTestResult[i].IsClosed)
				{
					temp = (pSwitchTestResult[i].CloseTemp );
					if(temp > max_temp)
					{
						max_temp = temp;
					}

					if(temp < min_temp)
					{
						min_temp = temp;
					}

					if(temp < temp_range.close_temp_min )
					{
						temp = x_min;
					}
					else if(temp > temp_range.close_temp_max)
					{
						temp = x_max;
					}

				}
				else
				{
					temp = x_max;
				}
			}
			if(temp < 200)
			{
				total[(short)temp] ++;
			}
		}
	}

	CString start_time;
	start_time.Format(_T("测试时间 %04d-%02d-%02d  %02d:%02d:%02d  "),
		BeginTestTime.GetYear(),
		BeginTestTime.GetMonth(),
		BeginTestTime.GetDay(),
		BeginTestTime.GetHour(),
		BeginTestTime.GetMinute(),
		BeginTestTime.GetSecond());

	 CBitmap bitmap;	
	 CBitmap* pOldBitmap;
	 CDC MemDC;
	 
	 CFont font,*oldFont;
	 CBrush groundbrush(COLORREF(RGB(0xFF,0xFF,0xFF)));
	 CBrush redbrush(COLORREF(RGB(0xFF,0,0)));
	CBrush bluebrush(COLORREF(RGB(0,0xFF,0)));
	 CBrush graybrush(COLORREF(RGB(192,192,192)));
	 CBrush* oldbrush;
	
	//绘制工具资源
	 CPen	pen;
	 CPen*	oldpen;
	
	//绘制准备
//	dc=this->GetDC();
		

	if(dc == NULL)
	{
		return;
	}

	
	if(!MemDC.CreateCompatibleDC(dc)) 						//建立与显示设备兼容的内存设备场境
	{
		return;
	}
	

	if(bitmap.GetSafeHandle() != NULL)
	{
		if(!bitmap.DeleteObject())
		{
			return;
		}
	}
	if(!bitmap.CreateCompatibleBitmap(dc,ChartRect.Width(),ChartRect.Height()))	//建立与显示设备兼容的位图
	{
		bitmap.DeleteObject(); 
		return;
	}

	pOldBitmap=MemDC.SelectObject(&bitmap); 			//将位图选入内存场境

#define PEN_WEIGHT	1	
	pen.CreatePen(PS_SOLID,PEN_WEIGHT, RGB(0x00,0x00,0x00));  		//建立画笔
	oldpen=MemDC.SelectObject(&pen);					//选择画笔对象
	

	

	//background
	oldbrush=MemDC.SelectObject(&groundbrush);				//选择画刷
	MemDC.Rectangle(0,0, ChartRect.Width(),ChartRect.Height());
	//MemDC.FillRect(CRect(0,0, ChartRect.Width(),ChartRect.Height()),
		
	//左边空1个FontSize.cx, 上面空 2 *FontSize.cy,	下面空 3 *FontSize.cy,
		
		
		CString s;
		MemDC.SetTextColor(RGB(0xFF,0xFF,0xFF));
		MemDC.SetBkMode(TRANSPARENT);
		
		CSize FontSize;

		s.Format(_T("最高温度:%5.1f℃ 最低温度:%5.1f℃"),max_temp,min_temp);
		s = start_time + s;

		int FontHeight = (ChartRect.Height()*10)/60;
		if(font.CreatePointFont(FontHeight,_T("黑体"),&MemDC))
		{
			oldFont = MemDC.SelectObject(&font);
		}
		else
		{
			return;
		}
		
		while(1)
		{
			 FontSize = MemDC.GetTextExtent(s);
			 if(FontSize.cx < ChartRect.Width() * 0.8)
			 {
				 break;
			 }
			 else
			 {
				MemDC.SelectObject(oldFont);
				font.DeleteObject(); 
				FontHeight -= 20;
				if(font.CreatePointFont(FontHeight,_T("黑体"),&MemDC))
				{
					oldFont = MemDC.SelectObject(&font);
				}
				else
				{
					return;
				}
			 }
			 
		}

		FontSize = MemDC.GetTextExtent(_T("88.8个"));

		
		int x_gap;
		

		x_gap = x_max - x_min + 1;
		


		pen.DeleteObject(); 
		pen.CreatePen(PS_DOT,PEN_WEIGHT, RGB(0x00,0x00,0x00));  		//建立画笔
		oldpen=MemDC.SelectObject(&pen);
		
		MemDC.SetTextColor(RGB(0,0,0));

		MemDC.DrawTextW(s,CRect(0,0,ChartRect.Width(),FontSize.cy * 2),DT_CENTER | DT_VCENTER);

		

		for(int i = 0; i <= 10; i++)
		{
			s.Format(_T("% 3d%%"),(10-i) * 10);
			CRect dt_rect;
			dt_rect.left   = 0;
			dt_rect.right  = FontSize.cx;
			dt_rect.top    = (long)((ChartRect.Height() -  FontSize.cy * 5)* i / 10+ FontSize.cy * 2 -  FontSize.cy * 0.5);
			dt_rect.bottom = (long)((ChartRect.Height() -  FontSize.cy * 5)* i / 10+ FontSize.cy * 2 +  FontSize.cy * 0.5);

			MemDC.DrawTextW(s,dt_rect,DT_CENTER);

			MemDC.MoveTo(FontSize.cx, (ChartRect.Height() -  FontSize.cy * 5)* i / 10 + FontSize.cy * 2); 	
			MemDC.LineTo(ChartRect.Width(), (ChartRect.Height() -  FontSize.cy * 5) * i / 10 + FontSize.cy * 2);
		}

		MemDC.MoveTo(FontSize.cx, FontSize.cy * 2); 	
		MemDC.LineTo(FontSize.cx, (ChartRect.Height() -  FontSize.cy * 3));
		
		

		CFont PCS_font;
		LOGFONT logFont;
		font.GetLogFont(&logFont);

		if(FontDirection)
		{
			PCS_font.CreateFont(-14,-7,90*10,0,400,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,

					  CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_ROMAN,_T("宋体"));
		}
		else
		{
			PCS_font.CreateFont(-14,-7,0*10,0,400,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,

                  CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_ROMAN,_T("宋体"));
		}

				
		//oldFont = MemDC.SelectObject(&font_90d);

		oldFont = MemDC.SelectObject(&PCS_font);

		for(int k = 0; k < x_min; k++)
		{
			total[(int)x_min] += total[k];
		}
		for(int k = x_max+1; k < 200; k++)
		{
			total[(int)x_max] += total[k];
		}

		for(int i = 0; i <=(int) x_gap; i++)
		{

			CRect rect;
			CRect dt_rect;
			if(FontDirection)
			{
				rect.left   = FontSize.cx + i * (ChartRect.Width() - FontSize.cx) / x_gap;
				rect.right  = rect.left + (ChartRect.Width() - FontSize.cx) / x_gap - FontSize.cx / 4;
				rect.top    = ChartRect.Height() - FontSize.cy * 3 - total[(int)x_min + i] * (ChartRect.Height() - FontSize.cy * 5) / sum_total;  
				rect.bottom = ChartRect.Height() - FontSize.cy * 3;

				

				if(total[(int)x_min + i] > 0)
				{
					s.Format(_T("%dPcs %3.1f%%"),total[(int)x_min + i],total[(int)x_min + i]*100.0f/sum_total);
					dt_rect.left   = rect.left;
					dt_rect.right  = dt_rect.left+FontSize.cy*2;
					dt_rect.top    = rect.top -  FontSize.cx*3;
					dt_rect.bottom = rect.top;
					MemDC.DrawTextW(s,dt_rect,DT_SINGLELINE|DT_BOTTOM);
				}
			}
			else
			{
				rect.left   = FontSize.cx + i * (ChartRect.Width() - FontSize.cx) / x_gap;
				rect.right  = rect.left + (ChartRect.Width() - FontSize.cx) / x_gap - FontSize.cx / 4;
				rect.top    = ChartRect.Height() - FontSize.cy * 3 - total[(int)x_min + i] * (ChartRect.Height() - FontSize.cy * 5) / sum_total;  
				rect.bottom = ChartRect.Height() - FontSize.cy * 3;

			
				if(total[(int)x_min + i] > 0)
				{
					s.Format(_T("%dPcs%3.1f%%"),total[(int)x_min + i],total[(int)x_min + i]*100.0f/sum_total);
					dt_rect.left   = rect.left;
					dt_rect.right  = dt_rect.left+FontSize.cy*10;
					dt_rect.top    = rect.top -  FontSize.cx;
					dt_rect.bottom = rect.top;
					MemDC.DrawTextW(s,dt_rect,DT_BOTTOM);
				}
			}

			
			

			
			
			if(i == 0)
			{
				s.Format(_T("<")) ;
			}
			else if((x_min+i) == x_max)
			{
				s.Format(_T(">")) ;
			}
			else
			{
				s.Format(_T("%d"),(int)(x_min+i)) ;
			}
			
			dt_rect.left   = rect.left;
			dt_rect.right  = dt_rect.left+FontSize.cy*2;
			dt_rect.top    = rect.bottom;
			dt_rect.bottom = rect.bottom + FontSize.cx;
			
			MemDC.DrawTextW(s,dt_rect, DT_SINGLELINE|DT_CENTER|DT_BOTTOM);//DT_LEFT);

			if((x_min+i) == x_max)
			{
				MemDC.FillRect(rect,&redbrush);
			}
			else if( i == 0)
			{
				MemDC.FillRect(rect,&redbrush);
			}
			else
			{
				MemDC.FillRect(rect,&bluebrush);
			}

			
			
		}

		MemDC.SelectObject(oldFont);
		PCS_font.DeleteObject();


	dc->BitBlt(ChartRect.left,ChartRect.top,ChartRect.Width(),ChartRect.Height(),&MemDC,0,0,SRCCOPY); 	//显示原图形


	MemDC.SelectObject(oldbrush);

	if(redbrush.GetSafeHandle() != NULL)
	{
		redbrush.DeleteObject();
	}
	if(graybrush.GetSafeHandle() != NULL)
	{
		graybrush.DeleteObject(); 
	}
	if(bluebrush.GetSafeHandle() != NULL)
	{
		bluebrush.DeleteObject(); 
	}
	
	if(groundbrush.GetSafeHandle() != NULL)
	{
		groundbrush.DeleteObject(); 
	}

	if(graybrush.GetSafeHandle() != NULL)
	{
		graybrush.DeleteObject(); 
	}

	
	MemDC.SelectObject(pOldBitmap);
	if(bitmap.GetSafeHandle() != NULL)
	{
		bitmap.DeleteObject(); 
	}
	MemDC.SelectObject(oldpen);
	if(pen.GetSafeHandle() != NULL)
	{
		pen.DeleteObject();
	}
	if(MemDC.GetSafeHdc() != NULL)
	{
		MemDC.DeleteDC();
	}
	
	//ReleaseDC(dc); 
}

#ifndef WINCE

BOOL CreateBMPFile(HWND hwnd, LPTSTR pszFile, HBITMAP hBMP, HDC hDC)  
{  
    PBITMAPINFO pbi = CreateBitmapInfoStruct(hwnd, hBMP);  
  
    HANDLE hf;                 // file handle   
    BITMAPFILEHEADER hdr;       // bitmap file-header   
    PBITMAPINFOHEADER pbih;     // bitmap info-header   
    LPBYTE lpBits;              // memory pointer   
    DWORD dwTotal;              // total count of bytes   
    DWORD cb;                   // incremental count of bytes   
    BYTE *hp;                   // byte pointer   
    DWORD dwTmp;   
  
    pbih = (PBITMAPINFOHEADER) pbi;   
    lpBits = (LPBYTE) GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);  
  
    if (!lpBits)   
	{
         AfxMessageBox(_T("GlobalAlloc Error"));   
	}
  
    // Retrieve the color table (RGBQUAD array) and the bits   
    // (array of palette indices) from the DIB.   
    if (!GetDIBits(hDC, hBMP, 0, (WORD) pbih->biHeight, lpBits, pbi,   
        DIB_RGB_COLORS))   
    {  
        AfxMessageBox(_T("GetDIBits Error"));   
    }  
  
    // Create the .BMP file.   
    hf = CreateFile(pszFile,   
                   GENERIC_READ | GENERIC_WRITE,   
                   (DWORD) 0,   
                    NULL,   
                   CREATE_ALWAYS,   
                   FILE_ATTRIBUTE_NORMAL,   
                   (HANDLE) NULL);   
    if (hf == INVALID_HANDLE_VALUE)   
        AfxMessageBox(_T("CreateFile"));   
    hdr.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M"   
    // Compute the size of the entire file.   
    hdr.bfSize = (DWORD) (sizeof(BITMAPFILEHEADER) +   
                 pbih->biSize + pbih->biClrUsed   
                 * sizeof(RGBQUAD) + pbih->biSizeImage);   
    hdr.bfReserved1 = 0;   
    hdr.bfReserved2 = 0;   
  
    // Compute the offset to the array of color indices.   
    hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) +   
                    pbih->biSize + pbih->biClrUsed   
                    * sizeof (RGBQUAD);   
  
    // Copy the BITMAPFILEHEADER into the .BMP file.   
    if (!WriteFile(hf, (LPVOID) &hdr, sizeof(BITMAPFILEHEADER),   
        (LPDWORD) &dwTmp,  NULL))   
    {  
       AfxMessageBox(_T("WriteFile"));   
    }  
  
    // Copy the BITMAPINFOHEADER and RGBQUAD array into the file.   
    if (!WriteFile(hf, (LPVOID) pbih, sizeof(BITMAPINFOHEADER)   
                  + pbih->biClrUsed * sizeof (RGBQUAD),   
                  (LPDWORD) &dwTmp, ( NULL)))  
    {  
        AfxMessageBox(_T("WriteFile"));   
    }  
          
  
    // Copy the array of color indices into the .BMP file.   
    dwTotal = cb = pbih->biSizeImage;   
    hp = lpBits;   
    if (!WriteFile(hf, (LPSTR) hp, (int) cb, (LPDWORD) &dwTmp,NULL))   
    {  
		AfxMessageBox(_T("WriteFile"));   
    }  
  
    // Close the .BMP file.   
     if (!CloseHandle(hf))  
	 {
		AfxMessageBox(_T("CloseHandle"));   
	 }
  
    // Free memory.   
    GlobalFree((HGLOBAL)lpBits);  
  
    return TRUE;  
}  
#endif

//创建一个BMP文件头对象  
PBITMAPINFO CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp)  
{  
    BITMAP bmp;   
    PBITMAPINFO pbmi;   
    WORD    cClrBits;   
  
    // Retrieve the bitmap color format, width, and height.   
    if (!GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp))   
	{
        AfxMessageBox(_T("GetObject"));   
	}
  
    // Convert the color format to a count of bits.   
    cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);   
    if (cClrBits == 1)   
        cClrBits = 1;   
    else if (cClrBits <= 4)   
        cClrBits = 4;   
    else if (cClrBits <= 8)   
        cClrBits = 8;   
    else if (cClrBits <= 16)   
        cClrBits = 16;   
    else if (cClrBits <= 24)   
        cClrBits = 24;   
    else cClrBits = 32;   
  
    // Allocate memory for the BITMAPINFO structure. (This structure   
    // contains a BITMAPINFOHEADER structure and an array of RGBQUAD   
    // data structures.)   
  
     if (cClrBits != 24)   
         pbmi = (PBITMAPINFO) LocalAlloc(LPTR,   
                    sizeof(BITMAPINFOHEADER) +   
                    sizeof(RGBQUAD) * (1<< cClrBits));   
  
     // There is no RGBQUAD array for the 24-bit-per-pixel format.   
  
     else   
         pbmi = (PBITMAPINFO) LocalAlloc(LPTR,   
                    sizeof(BITMAPINFOHEADER));   
  
    // Initialize the fields in the BITMAPINFO structure.   
  
    pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);   
    pbmi->bmiHeader.biWidth = bmp.bmWidth;   
    pbmi->bmiHeader.biHeight = bmp.bmHeight;   
    pbmi->bmiHeader.biPlanes = bmp.bmPlanes;   
    pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;   
    if (cClrBits < 24)   
        pbmi->bmiHeader.biClrUsed = (1<<cClrBits);   
  
    // If the bitmap is not compressed, set the BI_RGB flag.   
    pbmi->bmiHeader.biCompression = BI_RGB;   
  
    // Compute the number of bytes in the array of color   
    // indices and store the result in biSizeImage.   
    // For Windows NT, the width must be DWORD aligned unless   
    // the bitmap is RLE compressed. This example shows this.   
    // For Windows 95/98/Me, the width must be WORD aligned unless the   
    // bitmap is RLE compressed.  
    pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * cClrBits +31) & ~31) /8  
                                  * pbmi->bmiHeader.biHeight;   
    // Set biClrImportant to 0, indicating that all of the   
    // device colors are important.   
     pbmi->bmiHeader.biClrImportant = 0;   
     return pbmi;   
}  

extern CString G_sCompany;
extern CString G_sDefaultCompany;
void PrintReport_bmp(CDC *dc,CRect page_rect,CString Title ,int BoxNr, CTime BeginTime,CTime EndTime,
				 CString TestParaName,CString ProduceType,CString TestMemo,SWITCH_CONFIG_PARA ConfigPara,SWITCH_TEST_RESULT_EX* TestResult,BOOL bTestCloseLevel,int start_switch_nr,int end_switch_nr,BOX_TYPE BoxType,int CurGroupNr)
{
	Title = _T("热保护器测试报告");
//#define MAX_LED_NR_128	
	CFont *font=dc->GetCurrentFont();  
	LOGFONT logfont; 
	font->GetLogFont(&logfont); 

	
	CString str;
	
	
	
	CBrush whitebrush(COLORREF(RGB(255,255,255)));
	dc->FillRect(page_rect,&whitebrush);

	int FontPoint = page_rect.right / 2;

	
	CFont newFont;
	CFont* oldFont;
	CSize chnSize;
	while(1)
	{
		newFont.CreatePointFont(FontPoint,_T("宋体"),dc);
		oldFont = dc->SelectObject(&newFont);
		chnSize = dc->GetTextExtent(_T("编号 动作温 复位温 判定结果 "));
		if(page_rect.right  >= (4 *  chnSize.cx))
		{
			break;
		}
		FontPoint -= 20;
		newFont.DeleteObject();
		dc->SelectObject(oldFont);
	}
	oldFont->DeleteObject();
	
	CPen pen(PS_SOLID,chnSize.cy / 8,RGB(0x00,0x00,0x00));
	CPen *oldPen = dc->SelectObject(&pen);
	oldPen->DeleteObject();
	
	chnSize = dc->GetTextExtent(_T("国"));
	
	chnSize.cy  = chnSize.cy *1.5;
	int cur_x,cur_y;
	cur_x = page_rect.right/3;
	cur_y = chnSize.cy;

#ifdef WINCE
					dc->DrawText(Title,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, Title);
#endif


	G_sCompany = AfxGetApp()->GetProfileString(_T("SETTING"),_T("USER"),G_sDefaultCompany);


	CString sCompany = _T("公司名称:");
	cur_x = 0;
	cur_y += (int)(chnSize.cy *1.5);
	
	sCompany += G_sCompany;

#ifdef WINCE
					dc->DrawText(sCompany,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, sCompany);
#endif
	cur_x = 0;
	cur_y += (int)(chnSize.cy *1.5);
	

	CString sVal;
	//产品规格
	sVal = _T("产品规格:") + TestParaName; 
#ifdef WINCE
					dc->DrawText(sVal,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, sVal);
#endif
	//产品类型
	sVal = _T("产品类型:")+ProduceType;
#ifdef WINCE
					dc->DrawText(sVal,CRect(cur_x+chnSize.cx * 16,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x+chnSize.cx * 16,cur_y, sVal);
#endif
	
	sVal = _T("测试说明:")+TestMemo;
#ifdef WINCE
					dc->DrawText(sVal,CRect(cur_x+chnSize.cx * 32,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x+chnSize.cx * 32,cur_y, sVal);
#endif

	cur_x = 0;
	cur_y += (int)(chnSize.cy *1.5);
	str.Format(_T("烘箱编号: %d"), BoxNr + 1); 
	str += _T("  开始测试时间: ");
	sVal.Format(_T("%04d-%02d-%02d %02d:%02d"),BeginTime.GetYear(),BeginTime.GetMonth(),BeginTime.GetDay(),BeginTime.GetHour(),BeginTime.GetMinute());
	str += sVal;
	str += _T("   结束测试时间: ");
	sVal.Format(_T("%04d-%02d-%02d %02d:%02d"),EndTime.GetYear(),EndTime.GetMonth(),EndTime.GetDay(),EndTime.GetHour(),EndTime.GetMinute()); 
	str += sVal;
#ifdef WINCE
					dc->DrawText(str,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, str);
#endif

	CSize nr_Size = dc->GetTextExtent(_T("编号 "));
	CSize open_temp_Size = dc->GetTextExtent(_T("动作温 "));
	CSize close_temp_Size = dc->GetTextExtent(_T("复位温 "));
	CSize level_Size = dc->GetTextExtent(_T("判定结果 "));

	cur_x = 0;
	cur_y += (int)(chnSize.cy);
	str = _T("编号 动作温 复位温 判定结果 ");
	CSize title_Size = dc->GetTextExtent(str);
	int prn_col = page_rect.right / title_Size.cx;
	int i;

	for(i = 0; i < prn_col; i++)
	{
#ifdef WINCE
					dc->DrawText(str,CRect(cur_x,cur_y + chnSize.cy*0.3,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y + chnSize.cy * 0.3, str);
#endif
		cur_x += page_rect.right / prn_col;
	}

	int table_start_y = cur_y;

	cur_y += (int)(chnSize.cy *1.5);
	

	int prn_row = (int)((page_rect.bottom - cur_y) / (chnSize.cy *1.0));

	if(prn_row > (128 / prn_col))
	{
		prn_row = (128 / prn_col);
	}

	double open_max_temp = 0.0f;
	double open_min_temp = 999.0f;
	double close_max_temp = 0.0f;
	double close_min_temp = 999.0f;
	//double max_open_temp_level;
	//double min_open_temp_level;
	//double max_close_temp_level;
	//double min_close_temp_level;
	
	CONTROL_TEMP_RANGE temp_range;

	double max_temp = -999;
	double min_temp =  999;

	
	CheckHeatOrCoolMode(BoxNr,ConfigPara,&temp_range,BoxType);

//	max_open_temp_level = ConfigPara.HighLevel+ConfigPara.HighLevelSpanPlus;
//	min_open_temp_level = ConfigPara.LowLevel -ConfigPara.LowLevelSpanMinus; 

//	max_close_temp_level = ConfigPara.CloseLevel+ConfigPara.CloseLevelSpanPlus;
//	min_close_temp_level = ConfigPara.CloseLevel-ConfigPara.CloseLevelSpanMinus; 

	int count = 0;

	for(i=0; i < G_iMaxLedNr[BoxNr] / 8 ; i++)
	{
		count = 0;
		for(int j = 0;j < 8; j++)
		{
			if(TestResult[i* 8 + j].result.IsCloseFlash)
			{
				count ++;
			}
		}
		if( count >= 6)
		{
			for(int j = 0;j < 8; j++)
			{
				TestResult[i* 8 + j].result.IsCloseFlash = false;
			}
		}

		count = 0;
		for(int j = 0;j < 8; j++)
		{
			if(TestResult[i* 8 + j].result.IsOpenFlash)
			{
				count ++;
			}
		}
		if( count >= 4)
		{
			for(int j = 0;j < 8; j++)
			{
				TestResult[i* 8 + j].result.IsOpenFlash = false;
			}
		}
	}



	int   main_level = 0,total_switch = 0;

	for(i = 0; i < prn_row; i++)
	{
		for(int j = 0; j < prn_col; j++)
		{
			cur_x = j * page_rect.right / prn_col;
			if(i+prn_row*j <= end_switch_nr - start_switch_nr)

			{
				int index = i+prn_row * j + start_switch_nr;
				
				if(G_iMaxLedNr[BoxNr] == 192)
				{
					if((TestResult[index].ID % 12) < 9)
					{
						str.Format(_T("%02d%d"),TestResult[index].ID /12,(TestResult[index].ID%12)+1 );				
					}
					else if((TestResult[index].ID % 12) == 9)
					{
						str.Format(_T("%02d0"),TestResult[index].ID/12 + 1);				
					}
					else if((TestResult[index].ID % 12) == 10)
					{
						str.Format(_T("%02dA"),TestResult[index].ID/12 + 1);				
					}
					else if((TestResult[index].ID % 12) == 11)
					{
						str.Format(_T("%02dB"),TestResult[index].ID/12 +1);				
					}
				}
				else
				{
					str.Format(_T("%3d"),TestResult[index].ID+1); 
				}
				
#ifdef WINCE
					dc->DrawText(str,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, str);
#endif
				cur_x += nr_Size.cx;
				
				CONTROL_TEMP_RANGE temp_range;
				HEAT_COOL_ORDER	HeatOrCoolOrder  = CheckHeatOrCoolMode(BoxNr,ConfigPara,&temp_range,BoxType);

				switch(AfxGetApp()->GetProfileInt(_T("SETTING"),_T("PRINT TYPE"),0))
				{
				case 1:
					switch(CheckTestLevel(ConfigPara,TestResult[index].result,HeatOrCoolOrder,temp_range,TestResult->result,BoxType))
					{
					case MAIN_LEVEL:
					case TEST_RESULT_HIGH_LEVEL:
					case LOW_LEVEL:
						break;
				
					default:
						continue						;
					}
					break;
				case 2:
					switch(CheckTestLevel(ConfigPara,TestResult[index].result,HeatOrCoolOrder,temp_range,TestResult->result,BoxType))
					{
					case MAIN_LEVEL:
						break;
					default:
						continue						;
					}
					break;
				
				default:
					
					break;
				}
				double open_temp = TestResult[index].result.OpenTemp;
				str.Format(_T("%5.1f"),open_temp);
				if(open_temp <= 0.0f || open_temp >= 300.0f)
				{
					str = "-";
				}
				else if(open_temp > temp_range.open_temp_max || open_temp < temp_range.open_temp_min)
				{
					str +="X";
				}
				
				if(TestResult[index].result.IsOpenFlash)
				{
					if(TestResult[index].result.IsUsed)
					{
						str += "V";
					}
				}
#ifdef WINCE
					dc->DrawText(str,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, str);
#endif
				cur_x += open_temp_Size.cx;
				
				
				double close_temp = TestResult[index].result.CloseTemp;;
				str.Format(_T("%5.1f"),close_temp);
				if(close_temp <= 0.0f || close_temp >= 300.0f)
				{
					str = _T("");
				}
				else if(close_temp > temp_range.close_temp_max  || close_temp < temp_range.close_temp_min)
				{
					str +=_T("X");
				}

				if(TestResult[index].result.IsCloseFlash)
				{
					if(TestResult[index].result.IsUsed) 
						str += _T("V");
				}
#ifdef WINCE
					dc->DrawText(str,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, str);
#endif
				cur_x += close_temp_Size.cx;
				
				switch(CheckTestLevel(ConfigPara,TestResult[index].result,HeatOrCoolOrder,temp_range,TestResult->result,BoxType))
				{
				case MAIN_LEVEL:
					main_level ++;
					str =_T( "");//"主规格";
					break;
				case OVER_HIGH_LEVEL:
					str = _T("高温品");
					break;
				case OVER_LOW_LEVEL:
					str = _T("低温品");
					break;
				case TEST_RESULT_HIGH_LEVEL:
					str =_T("高规格G");
					break;
				case LOW_LEVEL:
					str =_T("低规格D");
					break;
				case CLOSE_LOW_LEVEL:
					str = _T("复大品X");
					break;
				case CLOSE_HIGH_LEVEL:
					str = _T("复小品X");
					break;
				case OPEN_FLASH_LEVEL:
					str = _T("动闪V");
					break;
				case CLOSE_FLASH_LEVEL:
					str = _T("复闪V");
					break;
				case OPEN_DOOR_RESET_LEVEL:
					str = _T("开门复位X");
					break;
				case NOT_USED:
					str = _T("无产品");
					break;
				default:
					str = _T("高低复大小");
					break;
				}
				
#ifdef WINCE
					dc->DrawText(str,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, str);
#endif
				cur_x += level_Size.cx;
				str = "";

				if(NOT_USED == CheckTestLevel(ConfigPara,TestResult[index].result,HeatOrCoolOrder,temp_range,TestResult->result,BoxType))
				{}
				else
				{
					total_switch ++;
					if(open_temp > 0)
					{
						if(open_temp > open_max_temp)
						{
							open_max_temp = open_temp;
						}
						if(open_temp < open_min_temp)
						{
							open_min_temp = open_temp;
						}
					}
					if(close_temp > 0)
					{
						if(close_temp > close_max_temp)
						{
							close_max_temp = close_temp;
						}
						if(close_temp < close_min_temp)
						{
							close_min_temp = close_temp;
						}
					}
				}
				
			}
		}
		
		cur_y += (int)(chnSize.cy);
	}

	
	for(i = 0; i <= prn_row + 1; i++)
	{
		dc->MoveTo(0,table_start_y + i * chnSize.cy + chnSize.cy* 0.3);
		dc->LineTo(page_rect.right,table_start_y + i * chnSize.cy + chnSize.cy*0.3); 
	}
	
	
	for(i = 0; i < prn_col; i++)
	{
		cur_x = i*page_rect.right / prn_col;
		int start_y = table_start_y + chnSize.cy* 0.3;
		int end_y = table_start_y + (prn_row + 1) * chnSize.cy + chnSize.cy*0.3;
		dc->MoveTo(cur_x, start_y);
		dc->LineTo(cur_x, end_y);

		dc->MoveTo(cur_x+nr_Size.cx, start_y);
		dc->LineTo(cur_x+nr_Size.cx, end_y);

		dc->MoveTo(cur_x+nr_Size.cx+ open_temp_Size.cx, start_y);
		dc->LineTo(cur_x+nr_Size.cx+ open_temp_Size.cx, end_y);
		
		dc->MoveTo(cur_x+nr_Size.cx+ open_temp_Size.cx+ close_temp_Size.cx, start_y);
		dc->LineTo(cur_x+nr_Size.cx+ open_temp_Size.cx+ close_temp_Size.cx, end_y);

		dc->MoveTo((i+1)*page_rect.right / prn_col - 1, start_y);
		dc->LineTo((i+1)*page_rect.right / prn_col - 1, end_y);
		
	}
	
	
//	cur_y += (int)(chnSize.cy *1.0);


	int total_product = 0,total_main_level = 0;
	TONG_JI_INFO TongJiData;

	TongJi(&TongJiData,
		ConfigPara,TestResult,bTestCloseLevel,BoxType,BoxNr,CurGroupNr);
	
	table_start_y = cur_y;
	
	cur_x = chnSize.cx ;
	str = _T("产品数");
#ifdef WINCE
					dc->DrawText(str,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, str);
#endif
	cur_x += page_rect.right /6;
	str.Format(_T("%d"),total_product);
#ifdef WINCE
					dc->DrawText(str,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, str);
#endif

	cur_x += page_rect.right /6;
	str = _T("合格品");
#ifdef WINCE
					dc->DrawText(str,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, str);
#endif
	cur_x += page_rect.right /6;
	str.Format(_T("%d"),total_main_level);
#ifdef WINCE
					dc->DrawText(str,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, str);
#endif

	cur_x += page_rect.right /6;
	str = _T("合格率");
#ifdef WINCE
					dc->DrawText(str,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, str);
#endif
	cur_x += page_rect.right /6;
	if(total_product > 0)
	{
		str.Format(_T("%5.1f%%"),(float)(total_main_level*100.0f/total_product));
	}
	else
	{
		str = _T("   0.0%");
	}
#ifdef WINCE
					dc->DrawText(str,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, str);
#endif
	
	cur_y += (int)(chnSize.cy);

	cur_x = chnSize.cx;
	
	str = _T("动作最高温");
#ifdef WINCE
					dc->DrawText(str,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, str);
#endif
	
	cur_x += page_rect.right /6;
	if(total_product > 0)
	{
		str.Format(_T("%5.1f"),open_max_temp);
	}
	else
	{
		str = _T(" 0.0");
	}
#ifdef WINCE
					dc->DrawText(str,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, str);
#endif

	cur_x += page_rect.right /6;
	str = _T("动作最低温");
#ifdef WINCE
					dc->DrawText(str,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, str);
#endif
	cur_x += page_rect.right /6;
	if(total_product > 0  && open_min_temp < 400)
	{
		str.Format(_T("%5.1f"),open_min_temp);
	}
	else
	{
		str = _T(" 0.0");
	}
#ifdef WINCE
					dc->DrawText(str,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, str);
#endif

	cur_x += page_rect.right /6;
	str = _T("动作平均温");
#ifdef WINCE
					dc->DrawText(str,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, str);
#endif
	cur_x += page_rect.right /6;
	if(total_product > 0)
	{
		str.Format(_T("%5.1f"),TongJiData.open_avr_temp);
	}
	else
	{
		str = _T(" 0.0");
	}
#ifdef WINCE
					dc->DrawText(str,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, str);
#endif
	
	cur_y += (int)(chnSize.cy);

	cur_x = chnSize.cx;
	str = _T("复位最高温");
#ifdef WINCE
					dc->DrawText(str,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, str);
#endif
	cur_x += page_rect.right /6;
	if(total_product > 0)
	{
		str.Format(_T("%5.1f"),close_max_temp);
	}
	else
	{
		str = _T(" 0.0");
	}
#ifdef WINCE
					dc->DrawText(str,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, str);
#endif

	cur_x += page_rect.right /6;
	str = _T("复位最低温");
#ifdef WINCE
					dc->DrawText(str,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, str);
#endif
	cur_x += page_rect.right /6;
	if(total_product > 0 && close_min_temp < 400)
	{
		str.Format(_T("%5.1f"),close_min_temp);
	}
	else
	{
		str = _T(" 0.0");
	}
#ifdef WINCE
					dc->DrawText(str,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, str);
#endif

	cur_x += page_rect.right /6;
	str = _T("复位平均温");
#ifdef WINCE
					dc->DrawText(str,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, str);
#endif
	cur_x += page_rect.right /6;
	if(total_product > 0)
	{
		str.Format(_T("%5.1f"),TongJiData.close_avr_temp);
	}
	else
	{
		str = _T(" 0.0");
	}
#ifdef WINCE
					dc->DrawText(str,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, str);
#endif

	cur_y += (int)(chnSize.cy);

	//CPen pen2(PS_SOLID,chnSize.cy / 8,RGB(0x00,0x00,0x00));
	//oldPen = dc->SelectObject(&pen2);
	//oldPen->DeleteObject();

	for(i = 0; i <= 7; i++)
	{
		cur_x = i*page_rect.right / 6;
		int start_y = table_start_y;
		dc->MoveTo(cur_x, start_y);
		dc->LineTo(cur_x, cur_y);

		dc->MoveTo(cur_x+page_rect.right / 6 - 1, start_y);
		dc->LineTo(cur_x+page_rect.right / 6 - 1, cur_y);
	}

	for(i = 0; i <= 3; i++)
	{
		int start_y = table_start_y + i * chnSize.cy ;
		//int end_y = cur_y;
		dc->MoveTo(0, start_y);
		dc->LineTo(page_rect.right, start_y);
	}

	
	cur_y += (int)(chnSize.cy / 2);
	table_start_y = cur_y;

	cur_x = chnSize.cx;
	str = _T("主规格温度");
#ifdef WINCE
					dc->DrawText(str,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, str);
#endif
	cur_x += page_rect.right /6;
	str.Format(_T("%5.1f"),ConfigPara.MainLevel);
#ifdef WINCE
					dc->DrawText(str,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, str);
#endif
	cur_x += page_rect.right /6;
	str = _T("主规负偏差");
#ifdef WINCE
					dc->DrawText(str,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, str);
#endif
	cur_x += page_rect.right /6;
	str.Format(_T("%5.1f"),-ConfigPara.MainLevelSpanMinus);
#ifdef WINCE
					dc->DrawText(str,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, str);
#endif
	cur_x += page_rect.right /6;
	str = _T("主规正偏差");
#ifdef WINCE
					dc->DrawText(str,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, str);
#endif
	cur_x += page_rect.right /6;
	str.Format(_T("%5.1f"),ConfigPara.MainLevelSpanPlus);
#ifdef WINCE
					dc->DrawText(str,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, str);
#endif
	cur_y += (int)(chnSize.cy);
//
	cur_x = chnSize.cx;
	str = _T("复位温度");
#ifdef WINCE
					dc->DrawText(str,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, str);
#endif
	cur_x += page_rect.right /6;
	str.Format(_T("%5.1f"),ConfigPara.CloseLevel);
#ifdef WINCE
					dc->DrawText(str,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, str);
#endif
	cur_x += page_rect.right /6;
	str = _T("复位负偏差");
#ifdef WINCE
					dc->DrawText(str,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, str);
#endif
	cur_x += page_rect.right /6;
	str.Format(_T("%5.1f"),-ConfigPara.CloseLevelSpanMinus);
#ifdef WINCE
					dc->DrawText(str,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, str);
#endif
	cur_x += page_rect.right /6;
	str = _T("复位正偏差");
#ifdef WINCE
					dc->DrawText(str,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, str);
#endif
	cur_x += page_rect.right /6;
	str.Format(_T("%5.1f"),ConfigPara.CloseLevelSpanPlus);
#ifdef WINCE
					dc->DrawText(str,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, str);
#endif
	cur_y += (int)(chnSize.cy);
//
	cur_x = chnSize.cx;
	str = _T("高规格温度");
#ifdef WINCE
					dc->DrawText(str,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, str);
#endif
	cur_x += page_rect.right /6;
	str.Format(_T("%5.1f"),ConfigPara.HighLevel);
#ifdef WINCE
					dc->DrawText(str,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, str);
#endif
	cur_x += page_rect.right /6;
	str = _T("高规负偏差");
#ifdef WINCE
					dc->DrawText(str,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, str);
#endif
	cur_x += page_rect.right /6;
	str.Format(_T("%5.1f"),-ConfigPara.HighLevelSpanMinus);
#ifdef WINCE
					dc->DrawText(str,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, str);
#endif
	cur_x += page_rect.right /6;
	str = _T("高规正偏差");
#ifdef WINCE
					dc->DrawText(str,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, str);
#endif
	cur_x += page_rect.right /6;
	str.Format(_T("%5.1f"),ConfigPara.HighLevelSpanPlus);
#ifdef WINCE
					dc->DrawText(str,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, str);
#endif
	cur_y += (int)(chnSize.cy);

	cur_x = chnSize.cx;
	str = _T("复位温度");
#ifdef WINCE
					dc->DrawText(str,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, str);
#endif
	cur_x += page_rect.right /6;
	str.Format(_T("%5.1f"),ConfigPara.CloseHighLevel);
#ifdef WINCE
					dc->DrawText(str,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, str);
#endif
	cur_x += page_rect.right /6;
	str = _T("复位负偏差");
#ifdef WINCE
					dc->DrawText(str,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, str);
#endif
	cur_x += page_rect.right /6;
	str.Format(_T("%5.1f"),-ConfigPara.CloseHighLevelSpanMinus);
#ifdef WINCE
					dc->DrawText(str,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, str);
#endif
	cur_x += page_rect.right /6;
	str = _T("复位正偏差");
#ifdef WINCE
					dc->DrawText(str,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, str);
#endif
	cur_x += page_rect.right /6;
	str.Format(_T("%5.1f"),ConfigPara.CloseHighLevelSpanPlus);
#ifdef WINCE
					dc->DrawText(str,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, str);
#endif
	cur_y += (int)(chnSize.cy);

	//
		cur_x = chnSize.cx;
	str = _T("低规格温度");
#ifdef WINCE
					dc->DrawText(str,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, str);
#endif
	cur_x += page_rect.right /6;
	str.Format(_T("%5.1f"),ConfigPara.LowLevel);
#ifdef WINCE
					dc->DrawText(str,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, str);
#endif
	cur_x += page_rect.right /6;
	str = _T("低规负偏差");
#ifdef WINCE
					dc->DrawText(str,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, str);
#endif
	cur_x += page_rect.right /6;
	str.Format(_T("%5.1f"),-ConfigPara.LowLevelSpanMinus);
#ifdef WINCE
					dc->DrawText(str,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, str);
#endif
	cur_x += page_rect.right /6;
	str = _T("低规正偏差");
#ifdef WINCE
					dc->DrawText(str,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, str);
#endif
	cur_x += page_rect.right /6;
	str.Format(_T("%5.1f"),ConfigPara.LowLevelSpanPlus);
#ifdef WINCE
					dc->DrawText(str,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, str);
#endif
	cur_y += (int)(chnSize.cy);

	cur_x = chnSize.cx;
	str = _T("复位温度");
#ifdef WINCE
					dc->DrawText(str,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, str);
#endif
	cur_x += page_rect.right /6;
	str.Format(_T("%5.1f"),ConfigPara.CloseLowLevel);
#ifdef WINCE
					dc->DrawText(str,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, str);
#endif
	cur_x += page_rect.right /6;
	str = _T("复位负偏差");
#ifdef WINCE
					dc->DrawText(str,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, str);
#endif
	cur_x += page_rect.right /6;
	str.Format(_T("%5.1f"),-ConfigPara.CloseLowLevelSpanMinus);
#ifdef WINCE
					dc->DrawText(str,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, str);
#endif
	cur_x += page_rect.right /6;
	str = _T("复位正偏差");
#ifdef WINCE
					dc->DrawText(str,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, str);
#endif
	cur_x += page_rect.right /6;
	str.Format(_T("%5.1f"),ConfigPara.CloseLowLevelSpanPlus);
#ifdef WINCE
					dc->DrawText(str,CRect(cur_x,cur_y,chnSize.cx * str.GetLength(), chnSize.cy),DT_RIGHT | DT_VCENTER);
#else
					dc->TextOut(cur_x,cur_y, str);
#endif

	cur_y += (int)(chnSize.cy);

//	CPen pen3(PS_SOLID,chnSize.cy / 8,RGB(0x00,0x00,0x00));
//	oldPen = dc->SelectObject(&pen3);
//	oldPen->DeleteObject();

	for(i = 0; i <= 7; i++)
	{
		cur_x = i*page_rect.right / 6;
		int start_y = table_start_y;
		dc->MoveTo(cur_x, start_y);
		dc->LineTo(cur_x, cur_y);

		dc->MoveTo(cur_x+page_rect.right / 6 - 1, start_y);
		dc->LineTo(cur_x+page_rect.right / 6 - 1, cur_y);
	}
	for(i = 0; i <= 6; i++)
	{
		int start_y = table_start_y + i * chnSize.cy ;
		//int end_y = cur_y;
		dc->MoveTo(0, start_y);
		dc->LineTo(page_rect.right, start_y);
	}

	
	
	//dc->TextOut(page_rect.right/2,page_rect.bottom - chnSize.cy, "制表： 常州市润邦电子科技有限公司 TEL:018052531601,013915838598");

}