
// MFCApplication1Dlg.h: файл заголовка
//

#pragma once
#include <vector>
#include <afxwin.h>
#include "resource.h"
#include <mmsystem.h>

// Диалоговое окно CMFCApplication1Dlg
class CMFCApplication1Dlg : public CDialogEx
{
// Создание
public:
	CMFCApplication1Dlg(CWnd* pParent = nullptr);	// стандартный конструктор
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION1_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV




// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CSliderCtrl m_sliderSamples;
	CComboBox m_comboThickness;
	afx_msg void OnBnClickedButtonSelectColor();
	afx_msg void OnCbnSelchangeComboThickness();

private:
	std::vector<short> m_samples;
	int m_displaySamples = 1000;
	COLORREF m_lineColor = RGB(0, 0, 255);
	int m_lineThickness = 1;
	void LoadWaveFile(const CString& path);
public:
	afx_msg void OnBnClickedButtonSelectFile();
};
