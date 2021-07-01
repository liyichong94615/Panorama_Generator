
// MFCApplication1Dlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"


#include <vector>
#include <string.h>
#include <math.h>
#include "OptionDlg.h"
#include <stdio.h>
#include <time.h>
#include <iostream>
#include "opencv2/opencv_modules.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/stitching.hpp"

#ifdef HAVE_OPENCV_XFEATURES2D
#include "opencv2/xfeatures2d/nonfree.hpp"
#endif


#define TEMP_BUFFER_SIZE 1004
#ifdef _DEBUG
#define new DEBUG_NEW
#endif



using namespace std;
using namespace cv;
using namespace cv::detail; 
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplication1Dlg dialog



CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION1_DIALOG, pParent)
	, m_nMultiRd(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_nCounter = 0;
	f_name.clear();
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);



	DDX_Control(pDX, IDC_PRGBAR, m_prgBar);
	DDX_Control(pDX, IDC_PROLBL, m_proLable);


	DDX_Control(pDX, IDC_STARTSTBTN, m_pStStitchBtn);
	DDX_Control(pDX, IDC_EXPBTN, m_pExpBtn);
	DDX_Radio(pDX, IDC_MULTIRD, m_nMultiRd);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
ON_BN_CLICKED(IDC_IMPBTN, &CMFCApplication1Dlg::OnBnClickedImpbtn)
ON_BN_CLICKED(IDC_EXPBTN, &CMFCApplication1Dlg::OnBnClickedExpbtn)

ON_WM_DESTROY()



ON_BN_CLICKED(IDC_STARTSTBTN, &CMFCApplication1Dlg::OnBnClickedStartstbtn)
ON_BN_CLICKED(IDC_BTNHELP, &CMFCApplication1Dlg::OnBnClickedBtnhelp)
END_MESSAGE_MAP()


// CMFCApplication1Dlg message handlers

BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	
	m_prgBar.SetRange(0, 1000);
	m_prgBar.SetPos(0);
	m_proLable.SetWindowTextW(_T("Ready"));
	
	// TODO: Add extra initialization here
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMFCApplication1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMFCApplication1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);

		
}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMFCApplication1Dlg::CreateBitmapInfo(int w, int h, int bpp)
{
	if (m_pBitmapInfo != NULL)
	{
		delete[]m_pBitmapInfo;
		m_pBitmapInfo = NULL;
	}

	if (bpp == 8)
		m_pBitmapInfo = (BITMAPINFO*) new BYTE[sizeof(BITMAPINFO) + 255 * sizeof(RGBQUAD)];
	else // 24 or 32bit
		m_pBitmapInfo = (BITMAPINFO*) new BYTE[sizeof(BITMAPINFO)];

	m_pBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_pBitmapInfo->bmiHeader.biPlanes = 1;
	m_pBitmapInfo->bmiHeader.biBitCount = bpp;
	m_pBitmapInfo->bmiHeader.biCompression = BI_RGB;
	m_pBitmapInfo->bmiHeader.biSizeImage = 0;
	m_pBitmapInfo->bmiHeader.biXPelsPerMeter = 0;
	m_pBitmapInfo->bmiHeader.biYPelsPerMeter = 0;
	m_pBitmapInfo->bmiHeader.biClrUsed = 0;
	m_pBitmapInfo->bmiHeader.biClrImportant = 0;

	if (bpp == 8)
	{
		for (int i = 0; i < 256; i++)
		{
			m_pBitmapInfo->bmiColors[i].rgbBlue = (BYTE)i;
			m_pBitmapInfo->bmiColors[i].rgbGreen = (BYTE)i;
			m_pBitmapInfo->bmiColors[i].rgbRed = (BYTE)i;
			m_pBitmapInfo->bmiColors[i].rgbReserved = 0;
		}
	}

	m_pBitmapInfo->bmiHeader.biWidth = w;
	m_pBitmapInfo->bmiHeader.biHeight = -h;
}

void CMFCApplication1Dlg::DrawImage()
{
	CClientDC dc(GetDlgItem(IDC_RESPIC));

	CRect rect;
	GetDlgItem(IDC_RESPIC)->GetClientRect(&rect);

	SetStretchBltMode(dc.GetSafeHdc(), COLORONCOLOR);
	StretchDIBits(dc.GetSafeHdc(), 0, 0, rect.Width(), rect.Height(), 0, 0, m_matResImage.cols, m_matResImage.rows, m_matResImage.data, m_pBitmapInfo, DIB_RGB_COLORS, SRCCOPY);
}


void CMFCApplication1Dlg::OnBnClickedImpbtn()
{
	f_name.clear();

	CTime cTime = CTime::GetCurrentTime();
	
	
	
		CString str = _T("Image file(*.BMP, *.GIF, *.JPG, *.bmp, *.gif, *.jpg, *.tif, *.png) | *.BMP; *.GIF; *.JPG; *.bmp; *.gif; *.jpg; *.tif; *.png | All file(*.*) | *.* || ");
		
		CString File;

		CString strFileList;

		CFileDialog dlg(TRUE, NULL, NULL, OFN_ALLOWMULTISELECT, str, this);

		const int c_cMaxFiles = 400;

		const int c_cbBuffSize = (c_cMaxFiles * (MAX_PATH + 1)) + 1;

		dlg.GetOFN().lpstrFile = strFileList.GetBuffer(c_cbBuffSize);

		dlg.GetOFN().nMaxFile = c_cbBuffSize;
		int cnt = 0;
		CString fname[1500];
		if (dlg.DoModal() == IDOK)
		{
			for (POSITION pos = dlg.GetStartPosition(); pos != NULL;)
            {
				File = dlg.GetNextPathName(pos);
				fname[cnt++] = File;
			}

		}
		
		if (cnt == 0) return;
		m_proLable.SetWindowTextW(_T("Importing..."));
		CString temp;
		char file_path[10005];
		Mat img;
        int step = 1000 / cnt - 1;
		int curPos = 0;
		

		for (int i = 0; i < cnt; i++) {		
			temp = fname[i];
			WideCharToMultiByte(CP_ACP, 0, temp.GetBuffer(0), -1, file_path, TEMP_BUFFER_SIZE, 0, 0);
			f_name.push_back(file_path);
			curPos += step;
			m_prgBar.SetPos(curPos);
		}
		m_prgBar.SetPos(1000);
		m_proLable.SetWindowTextW(_T("Import Complete"));

}



void CMFCApplication1Dlg::OnBnClickedExpbtn()
{
	if (!m_matResImage.empty()) {
		static TCHAR BASED_CODE szFilter[] = _T("Image file(*.BMP, *.GIF, *.JPG, *.jpg, *.png, *.bmp, *.tiff, *.gif) | *.BMP;*.GIF;*.JPG;*.bmp;*.jpg;*.gif;*.png |All file(*.*)|*.*||");

		CFileDialog dlg(FALSE, _T("*.jpg"), _T("image"), OFN_HIDEREADONLY, szFilter);

		if (IDOK == dlg.DoModal())
		{
			char szTemp[100] = "";

			CString pathName = dlg.GetPathName();
			WideCharToMultiByte(CP_ACP, 0, pathName.GetBuffer(0), -1, szTemp, TEMP_BUFFER_SIZE, 0, 0);
			cv::imwrite(szTemp, m_matResImage);
			
			f_name.clear();

            #ifndef ONLINE_JUDGE
                FILE* stream;
                freopen_s(&stream, "report.txt", "w", stdout);
            #endif



            CTime data = GetCurrentTime();
            printf("Date: %d %d %d", data.GetYear(), data.GetMonth(), data.GetDay());
            MessageBox(_T("It was saved successfully."));
            f_name.clear();

		}

	}
	else {

		MessageBox(_T("You don't have result image. Please do stitch working."));
	}
}


//void CMFCApplication1Dlg::OnBnClickedBtnhelp()
//{
//   
//	CAboutDlg test;
//	test.DoModal();
//
//	// TODO: Add your control notification handler code here
//}





bool CMFCApplication1Dlg::CalStitching(int warp_no, int method) {
  
    if (method == 1) {
        string warp_type;
        switch (warp_no)
        {
        case 0:
            warp_type = "cylindrical";
            break;
        case 1:
            warp_type = "plane";
            break;
        case 2:
            warp_type = "stereographic";
            break;
        default:
            break;
        }
        bool preview = true;
        bool try_gpu = false;
        double work_megapix = 0.6;
        double seam_megapix = 0.1;
        double compose_megapix = -1;
        float conf_thresh = 1.f;
        string features_type = "surf";
        string ba_cost_func = "ray";
        string ba_refine_mask = "xxxxx";
        bool do_wave_correct = true;
        WaveCorrectKind wave_correct = detail::WAVE_CORRECT_HORIZ;

        int expos_comp_type = ExposureCompensator::GAIN_BLOCKS;
        float match_conf = 0.65f;
        string seam_find_type = "gc_color";
        int blend_type = Blender::MULTI_BAND;
        float blend_strength = 5;
        string result_name = "result.jpg";



        double work_scale = 1, seam_scale = 1, compose_scale = 1;
        bool is_work_scale_set = false, is_seam_scale_set = false, is_compose_scale_set = false;


        int num_images = f_name.size();

        Ptr<FeaturesFinder> finder;
        finder = new SurfFeaturesFinder();

        Mat full_img, img;

        vector<ImageFeatures> features(num_images);
        vector<Mat> images(num_images);
        vector<Size> full_img_sizes(num_images);
        double seam_work_aspect = 1;


        int step = 500 / num_images;
        int curPos = 0;


        for (int i = 0; i < num_images; ++i)
        {
            full_img = imread(f_name[i]);
            cv::resize(full_img, full_img, Size(1024, 768));

            full_img_sizes[i] = full_img.size();

            if (work_megapix < 0)
            {
                img = full_img;
                work_scale = 1;
                is_work_scale_set = true;
            }
            else
            {
                if (!is_work_scale_set)
                {
                    work_scale = min(1.0, sqrt(work_megapix * 1e6 / full_img.size().area()));
                    is_work_scale_set = true;
                }
                cv::resize(full_img, img, Size(), work_scale, work_scale);
            }
            if (!is_seam_scale_set)
            {
                seam_scale = min(1.0, sqrt(seam_megapix * 1e6 / full_img.size().area()));
                seam_work_aspect = seam_scale / work_scale;
                is_seam_scale_set = true;
            }

            (*finder)(img, features[i]);
            features[i].img_idx = i;
            cv::resize(full_img, img, Size(), seam_scale, seam_scale);
            images[i] = img.clone();
            curPos += step;
            m_prgBar.SetPos(curPos);
        }

        
        full_img.release();
        img.release();
        vector<MatchesInfo> pairwise_matches;
        BestOf2NearestMatcher matcher(try_gpu, match_conf);
        matcher(features, pairwise_matches);
        matcher.collectGarbage();


        // Leave only images we are sure are from the same panorama
        vector<int> indices = leaveBiggestComponent(features, pairwise_matches, conf_thresh);
        vector<Mat> img_subset;
        vector<string> img_names_subset;
        vector<Size> full_img_sizes_subset;
        for (size_t i = 0; i < indices.size(); ++i)
        {
            img_names_subset.push_back(f_name[indices[i]]);
            img_subset.push_back(images[indices[i]]);
            full_img_sizes_subset.push_back(full_img_sizes[indices[i]]);
        }

        images = img_subset;
        f_name = img_names_subset;
        full_img_sizes = full_img_sizes_subset;

        // Check if we still have enough images
        num_images = static_cast<int>(f_name.size());
        if (num_images < 2) {
            return false;
        }

        HomographyBasedEstimator estimator;
        vector<CameraParams> cameras;
        estimator(features, pairwise_matches, cameras);


        for (size_t i = 0; i < cameras.size(); ++i)
        {
            Mat R;
            cameras[i].R.convertTo(R, CV_32F);
            cameras[i].R = R;

        }

        Ptr<detail::BundleAdjusterBase> adjuster;
        if (ba_cost_func == "reproj") adjuster = new detail::BundleAdjusterReproj();
        else if (ba_cost_func == "ray") adjuster = new detail::BundleAdjusterRay();

        adjuster->setConfThresh(conf_thresh);
        Mat_<uchar> refine_mask = Mat::zeros(3, 3, CV_8U);
        if (ba_refine_mask[0] == 'x') refine_mask(0, 0) = 1;
        if (ba_refine_mask[1] == 'x') refine_mask(0, 1) = 1;
        if (ba_refine_mask[2] == 'x') refine_mask(0, 2) = 1;
        if (ba_refine_mask[3] == 'x') refine_mask(1, 1) = 1;
        if (ba_refine_mask[4] == 'x') refine_mask(1, 2) = 1;
        adjuster->setRefinementMask(refine_mask);
        (*adjuster)(features, pairwise_matches, cameras);

        // Find median focal length

        vector<double> focals;
        for (size_t i = 0; i < cameras.size(); ++i) focals.push_back(cameras[i].focal);
        sort(focals.begin(), focals.end());

        float warped_image_scale;
        if (focals.size() % 2 == 1)
            warped_image_scale = static_cast<float>(focals[focals.size() / 2]);
        else
            warped_image_scale = static_cast<float>(focals[focals.size() / 2 - 1] + focals[focals.size() / 2]) * 0.5f;

        if (do_wave_correct)
        {
            vector<Mat> rmats;
            for (size_t i = 0; i < cameras.size(); ++i)
                rmats.push_back(cameras[i].R.clone());
            waveCorrect(rmats, wave_correct);
            for (size_t i = 0; i < cameras.size(); ++i)
                cameras[i].R = rmats[i];
        }



        vector<Point> corners(num_images);
        vector<Mat> masks_warped(num_images);
        vector<Mat> images_warped(num_images);
        vector<Size> sizes(num_images);
        vector<Mat> masks(num_images);

        // Preapre images masks
        for (int i = 0; i < num_images; ++i)
        {
            masks[i].create(images[i].size(), CV_8U);
            masks[i].setTo(Scalar::all(255));
        }

        // Warp images and their masks

        Ptr<WarperCreator> warper_creator;
#if defined(HAVE_OPENCV_GPU)
        if (try_gpu && gpu::getCudaEnabledDeviceCount() > 0)
        {
            if (warp_type == "plane") warper_creator = new cv::PlaneWarperGpu();
            else if (warp_type == "cylindrical") warper_creator = new cv::CylindricalWarperGpu();
            else if (warp_type == "spherical") warper_creator = new cv::SphericalWarperGpu();
        }
        else
#endif
        {
            if (warp_type == "plane") warper_creator = new cv::PlaneWarper();
            else if (warp_type == "cylindrical") warper_creator = new cv::CylindricalWarper();
            else if (warp_type == "spherical") warper_creator = new cv::SphericalWarper();
            else if (warp_type == "fisheye") warper_creator = new cv::FisheyeWarper();
            else if (warp_type == "stereographic") warper_creator = new cv::StereographicWarper();
            else if (warp_type == "compressedPlaneA2B1") warper_creator = new cv::CompressedRectilinearWarper(2, 1);
            else if (warp_type == "compressedPlaneA1.5B1") warper_creator = new cv::CompressedRectilinearWarper(1.5, 1);
            else if (warp_type == "compressedPlanePortraitA2B1") warper_creator = new cv::CompressedRectilinearPortraitWarper(2, 1);
            else if (warp_type == "compressedPlanePortraitA1.5B1") warper_creator = new cv::CompressedRectilinearPortraitWarper(1.5, 1);
            else if (warp_type == "paniniA2B1") warper_creator = new cv::PaniniWarper(2, 1);
            else if (warp_type == "paniniA1.5B1") warper_creator = new cv::PaniniWarper(1.5, 1);
            else if (warp_type == "paniniPortraitA2B1") warper_creator = new cv::PaniniPortraitWarper(2, 1);
            else if (warp_type == "paniniPortraitA1.5B1") warper_creator = new cv::PaniniPortraitWarper(1.5, 1);
            else if (warp_type == "mercator") warper_creator = new cv::MercatorWarper();
            else if (warp_type == "transverseMercator") warper_creator = new cv::TransverseMercatorWarper();
        }



        Ptr<RotationWarper> warper = warper_creator->create(static_cast<float>(warped_image_scale * seam_work_aspect));

        for (int i = 0; i < num_images; ++i)
        {
            Mat_<float> K;
            cameras[i].K().convertTo(K, CV_32F);
            float swa = (float)seam_work_aspect;
            K(0, 0) *= swa; K(0, 2) *= swa;
            K(1, 1) *= swa; K(1, 2) *= swa;

            corners[i] = warper->warp(images[i], K, cameras[i].R, INTER_LINEAR, BORDER_REFLECT, images_warped[i]);
            sizes[i] = images_warped[i].size();

            warper->warp(masks[i], K, cameras[i].R, INTER_NEAREST, BORDER_CONSTANT, masks_warped[i]);
        }

        vector<Mat> images_warped_f(num_images);
        for (int i = 0; i < num_images; ++i)
            images_warped[i].convertTo(images_warped_f[i], CV_32F);



        Ptr<ExposureCompensator> compensator = ExposureCompensator::createDefault(expos_comp_type);
        compensator->feed(corners, images_warped, masks_warped);

        Ptr<SeamFinder> seam_finder;
        if (seam_find_type == "no")
            seam_finder = new detail::NoSeamFinder();
        else if (seam_find_type == "voronoi")
            seam_finder = new detail::VoronoiSeamFinder();
        else if (seam_find_type == "gc_color")
        {
#if defined(HAVE_OPENCV_GPU)
            if (try_gpu && gpu::getCudaEnabledDeviceCount() > 0)
                seam_finder = new detail::GraphCutSeamFinderGpu(GraphCutSeamFinderBase::COST_COLOR);
            else
#endif
                seam_finder = new detail::GraphCutSeamFinder(GraphCutSeamFinderBase::COST_COLOR);
        }
        else if (seam_find_type == "gc_colorgrad")
        {
#if defined(HAVE_OPENCV_GPU)
            if (try_gpu && gpu::getCudaEnabledDeviceCount() > 0)
                seam_finder = new detail::GraphCutSeamFinderGpu(GraphCutSeamFinderBase::COST_COLOR_GRAD);
            else
#endif
                seam_finder = new detail::GraphCutSeamFinder(GraphCutSeamFinderBase::COST_COLOR_GRAD);
        }
        else if (seam_find_type == "dp_color")
            seam_finder = new detail::DpSeamFinder(DpSeamFinder::COLOR);
        else if (seam_find_type == "dp_colorgrad")
            seam_finder = new detail::DpSeamFinder(DpSeamFinder::COLOR_GRAD);


        seam_finder->find(images_warped_f, corners, masks_warped);

        // Release unused memory
        images.clear();
        images_warped.clear();
        images_warped_f.clear();
        masks.clear();



        Mat img_warped, img_warped_s;
        Mat dilated_mask, seam_mask, mask, mask_warped;
        Ptr<Blender> blender;
        //double compose_seam_aspect = 1;
        double compose_work_aspect = 1;



        step = 500 / num_images;
        curPos = 500;

        for (int img_idx = 0; img_idx < num_images; ++img_idx)
        {


            // Read image and resize it if necessary
            full_img = imread(f_name[img_idx]);

            cv::resize(full_img, full_img, Size(1024, 768));
            if (!is_compose_scale_set)
            {
                if (compose_megapix > 0)
                    compose_scale = min(1.0, sqrt(compose_megapix * 1e6 / full_img.size().area()));
                is_compose_scale_set = true;

                // Compute relative scales
                //compose_seam_aspect = compose_scale / seam_scale;
                compose_work_aspect = compose_scale / work_scale;

                // Update warped image scale
                warped_image_scale *= static_cast<float>(compose_work_aspect);
                warper = warper_creator->create(warped_image_scale);

                // Update corners and sizes
                for (int i = 0; i < num_images; ++i)
                {
                    // Update intrinsics
                    cameras[i].focal *= compose_work_aspect;
                    cameras[i].ppx *= compose_work_aspect;
                    cameras[i].ppy *= compose_work_aspect;

                    // Update corner and size
                    Size sz = full_img_sizes[i];
                    if (std::abs(compose_scale - 1) > 1e-1)
                    {
                        sz.width = cvRound(full_img_sizes[i].width * compose_scale);
                        sz.height = cvRound(full_img_sizes[i].height * compose_scale);
                    }

                    Mat K;
                    cameras[i].K().convertTo(K, CV_32F);
                    Rect roi = warper->warpRoi(sz, K, cameras[i].R);
                    corners[i] = roi.tl();
                    sizes[i] = roi.size();
                }
            }
            if (abs(compose_scale - 1) > 1e-1)
                cv::resize(full_img, img, Size(), compose_scale, compose_scale);
            else
                img = full_img;
            full_img.release();
            Size img_size = img.size();

            Mat K;
            cameras[img_idx].K().convertTo(K, CV_32F);

            // Warp the current image
            warper->warp(img, K, cameras[img_idx].R, INTER_LINEAR, BORDER_REFLECT, img_warped);

            // Warp the current image mask
            mask.create(img_size, CV_8U);
            mask.setTo(Scalar::all(255));
            warper->warp(mask, K, cameras[img_idx].R, INTER_NEAREST, BORDER_CONSTANT, mask_warped);

            // Compensate exposure
            compensator->apply(img_idx, corners[img_idx], img_warped, mask_warped);

            img_warped.convertTo(img_warped_s, CV_16S);
            img_warped.release();
            img.release();
            mask.release();

            cv::dilate(masks_warped[img_idx], dilated_mask, Mat());
            cv::resize(dilated_mask, seam_mask, mask_warped.size());
            mask_warped = seam_mask & mask_warped;

            if (blender.empty())
            {
                blender = Blender::createDefault(blend_type, try_gpu);
                Size dst_sz = resultRoi(corners, sizes).size();
                float blend_width = sqrt(static_cast<float>(dst_sz.area()))* blend_strength / 100.f;
                if (blend_width < 1.f)
                    blender = Blender::createDefault(Blender::NO, try_gpu);
                else if (blend_type == Blender::MULTI_BAND)
                {
                    MultiBandBlender* mb = dynamic_cast<MultiBandBlender*>(static_cast<Blender*>(blender));
                    mb->setNumBands(static_cast<int>(ceil(log(blend_width) / log(2.)) - 1.));

                }
                else if (blend_type == Blender::FEATHER)
                {
                    FeatherBlender* fb = dynamic_cast<FeatherBlender*>(static_cast<Blender*>(blender));
                    fb->setSharpness(1.f / blend_width);

                }
                blender->prepare(corners, sizes);
            }

            // Blend the current image
            blender->feed(img_warped_s, mask_warped, corners[img_idx]);
            curPos += step;
            m_prgBar.SetPos(curPos);
        }

        Mat result, result_mask;
        blender->blend(result, result_mask);
        cv::imwrite(result_name, result);
        return true;

    }
    else {

        Mat result;
        string result_name = "result.jpg";
        int num_images = f_name.size();
        vector<Mat> m_vImages;

        Mat temp;

        int step = 500 / num_images - 1;
        int curPos = 0;
       
        for (int i = 0; i < num_images; i++) {
            curPos += step;
            m_prgBar.SetPos(curPos);
            temp = imread(f_name[i]);
            if (temp.empty()) return false;

            if (m_nMultiRd == 1) m_vImages.push_back(temp);
            else cv::resize(temp, temp, Size(400, 300));
            m_vImages.push_back(temp);
        }
        
        m_prgBar.SetPos(500);


        Mat pano;
        bool try_use_gpu = false;

        Stitcher stitcher = Stitcher::createDefault(try_use_gpu);
        Stitcher::Status status = stitcher.stitch(m_vImages, result);

        if (status != Stitcher::OK) return false;

        m_prgBar.SetPos(1000);
    
        cv::imwrite(result_name, result);
        m_vImages.clear();
        return true;
        
    }
}



void CMFCApplication1Dlg::OnBnClickedStartstbtn()
{
  
    UpdateData(true);
    if (f_name.size() != 0) {
        OptionDlg dlg;

        if (f_name.size() >= 2) {
            if (dlg.DoModal() == IDOK) {



                m_proLable.SetWindowTextW(_T("Stitching..."));
                m_prgBar.SetPos(0);
                m_pStStitchBtn.SetWindowTextW(_T("Stop Stitching"));
                bool result_flg = CalStitching(dlg.m_nRenOptRd, m_nMultiRd);
                if (!result_flg) {
                    MessageBox(_T("Can't stitching from choosen images"));
                    return;
                }
                Mat pano;
                pano = imread("result.jpg");
                m_matResImage = pano.clone();
                switch (dlg.m_nImgRotRd)
                {
                case 0:
                    break;
                case 1:
                    transpose(m_matResImage, m_matResImage);
                    break;
                case 2:
                    transpose(m_matResImage, m_matResImage);
                    flip(m_matResImage, m_matResImage, 1);
                    flip(m_matResImage, m_matResImage, 0);

                    break;
                case 3:
                    flip(m_matResImage, m_matResImage, 0);
                    break;
                default:
                    break;
                }

                cv::resize(m_matResImage, m_matResImage, Size(dlg.m_pWidthPxNum, dlg.m_pHeiPxNum));
                CreateBitmapInfo(m_matResImage.cols, m_matResImage.rows, m_matResImage.channels() * 8);
                DrawImage();
                m_prgBar.SetPos(1000);
                MessageBox(_T("Stitching was successful."));

                m_proLable.SetWindowTextW(_T("Ready"));
                m_prgBar.SetPos(0);
                m_pStStitchBtn.SetWindowTextW(_T("Start Stitching"));

            }
        }
        else {

            MessageBox(_T("Please import over 2 Images "));
            f_name.clear();

        }

    }
    else {
        MessageBox(_T("Please import Images to stitch"));
        f_name.clear();
        return;
    }
    // TODO: Add your control notification handler code here
}


void CMFCApplication1Dlg::OnBnClickedBtnhelp()
{
    CAboutDlg test;
    test.DoModal();
    // TODO: Add your control notification handler code here
}
