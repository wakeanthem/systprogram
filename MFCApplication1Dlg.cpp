
// MFCApplication1Dlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"
#include <mmsystem.h>    
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CMFCApplication1Dlg



CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION1_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_SLIDER_SAMPLES, m_sliderSamples);
    DDX_Control(pDX, IDC_COMBO_THICKNESS, m_comboThickness);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_SELECT_COLOR, &CMFCApplication1Dlg::OnBnClickedButtonSelectColor)
    ON_CBN_SELCHANGE(IDC_COMBO_THICKNESS, &CMFCApplication1Dlg::OnCbnSelchangeComboThickness)
    ON_WM_HSCROLL()
    ON_BN_CLICKED(IDC_BUTTON_SELECT_FILE, &CMFCApplication1Dlg::OnBnClickedButtonSelectFile)
END_MESSAGE_MAP()


// Обработчики сообщений CMFCApplication1Dlg

BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию


    // Установка начального положения
    m_sliderSamples.SetPos(1000);
    m_sliderSamples.SetRange(100, 20000);
    m_sliderSamples.SetPos(1000);

    m_comboThickness.AddString(_T("1"));
    m_comboThickness.AddString(_T("2"));
    m_comboThickness.AddString(_T("3"));
    m_comboThickness.AddString(_T("4"));
    m_comboThickness.AddString(_T("5"));
    m_comboThickness.SetCurSel(0);
	return TRUE; 
}


void CMFCApplication1Dlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this);
        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
        return;
    }

    CPaintDC dc(this);
    CRect rect;
    GetClientRect(&rect);

    dc.FillSolidRect(rect, RGB(255, 255, 255)); 

  

  
    if (!m_samples.empty())
    {
        size_t num = min(static_cast<size_t>(m_displaySamples), m_samples.size());
        if (num >= 2)
        {
            // Автомасштаб амплитуды
            short minVal = *std::min_element(m_samples.begin(), m_samples.begin() + num);
            short maxVal = *std::max_element(m_samples.begin(), m_samples.begin() + num);
            int ampRange = maxVal - minVal;
            if (ampRange == 0) ampRange = 1;

            // Область для графика
            int left = 50;
            int right = rect.right - 50;
            int top = 120;                
            int bottom = rect.bottom - 100; 
            int height = bottom - top;
            int zeroY = top + height / 2;

            // Ось времени
            CPen axisPen(PS_SOLID, 1, RGB(0, 0, 0));
            CPen* oldPen = dc.SelectObject(&axisPen);
            dc.MoveTo(left, bottom);
            dc.LineTo(right, bottom);
            dc.TextOut(left, bottom + 5, _T("0"));
            CString endText;
            endText.Format(_T("%d"), m_displaySamples);
            dc.TextOut(right - 70, bottom + 5, endText);
            dc.SelectObject(oldPen);

            // Линия сигнала
            CPen signalPen(PS_SOLID, m_lineThickness, m_lineColor);
            dc.SelectObject(&signalPen);

            double xStep = static_cast<double>(right - left) / (num - 1);

            int y = zeroY - static_cast<int>((m_samples[0] - minVal) * height / ampRange);
            dc.MoveTo(left, y);

            for (size_t i = 1; i < num; ++i)
            {
                y = zeroY - static_cast<int>((m_samples[i] - minVal) * height / ampRange);
                int x = left + static_cast<int>(i * xStep);
                dc.LineTo(x, y);
            }

            dc.SelectObject(oldPen);
        }
    }
    else
    {
        dc.TextOut(50, 200, _T("Выберите WAV-файл для отображения осциллограммы"));
    }
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void CMFCApplication1Dlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    if (pScrollBar == (CScrollBar*)&m_sliderSamples)  
    {
        m_displaySamples = m_sliderSamples.GetPos();
        Invalidate();  // перерисовать график
    }

    CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);  
}


void CMFCApplication1Dlg::OnBnClickedButtonSelectColor()
{
    CColorDialog dlg(m_lineColor);
    if (dlg.DoModal() == IDOK)
    {
        m_lineColor = dlg.GetColor();
    }
    Invalidate(); 
}

void CMFCApplication1Dlg::OnCbnSelchangeComboThickness()
{
    int sel = m_comboThickness.GetCurSel();
    if (sel != CB_ERR)
    {
        m_lineThickness = sel + 1; 
    }
    Invalidate();  
}
void CMFCApplication1Dlg::LoadWaveFile(const CString& path)
{
    m_samples.clear();

    CFile file;
    if (!file.Open(path, CFile::modeRead | CFile::typeBinary))
    {
        AfxMessageBox(_T("Не удалось открыть файл"));
        return;
    }

   
    char header[12];
    if (file.Read(header, 12) != 12 || memcmp(header, "RIFF", 4) != 0 || memcmp(header + 8, "WAVE", 4) != 0)
    {
        AfxMessageBox(_T("Не является WAV-файлом"));
        file.Close();
        return;
    }

    WAVEFORMATEX fmt = { 0 };
    bool fmtFound = false;

  
    while (file.GetPosition() < file.GetLength())
    {
        char chunkID[4];
        DWORD chunkSize = 0;

        if (file.Read(chunkID, 4) != 4 || file.Read(&chunkSize, 4) != 4)
            break;

        if (memcmp(chunkID, "fmt ", 4) == 0)
        {
            file.Read(&fmt, sizeof(WAVEFORMATEX));
            fmtFound = true;
            file.Seek(chunkSize - sizeof(WAVEFORMATEX), CFile::current);
        }
        else if (memcmp(chunkID, "data", 4) == 0)
        {
            if (!fmtFound || fmt.wFormatTag != WAVE_FORMAT_PCM || fmt.nChannels != 1 || fmt.wBitsPerSample != 16)
            {
                AfxMessageBox(_T("Поддерживается только mono 16-bit PCM WAV"));
                file.Close();
                return;
            }

            m_samples.resize(chunkSize / 2);
            file.Read(m_samples.data(), chunkSize);
            file.Close();
            Invalidate();
            return;  
        }
        else
        {
            file.Seek(chunkSize, CFile::current);  
        }
    }

    file.Close();
    AfxMessageBox(_T("Не найден чанк data или неподходящий формат"));
}


void CMFCApplication1Dlg::OnBnClickedButtonSelectFile()
{
   
    CFileDialog dlg(TRUE, _T("wav"), NULL, OFN_HIDEREADONLY, _T("WAV файлы (*.wav)|*.wav||"), this);
    if (dlg.DoModal() == IDOK)
    {
        LoadWaveFile(dlg.GetPathName());
        Invalidate();
    }
}
