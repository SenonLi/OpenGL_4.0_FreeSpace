#include "stdafx.h"
#include "SLOpenCVBasics.h"
#include <codecvt>              // for ws2s() and s2ws

namespace slopencv
{
	// slutil, statistic
	std::string ws2s(const std::wstring& wstr)
	{
		using convert_typeX = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_typeX, wchar_t> converterX;

		return converterX.to_bytes(wstr);
	}
	std::wstring s2ws(const std::string& str)
	{
		using convert_typeX = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_typeX, wchar_t> converterX;

		return converterX.from_bytes(str);
	}

	// slutil, statistic
	double GetRootMeanSquare(const std::vector<double>& dataVect)
	{
		if (dataVect.size() == 0)
			return -1.0;

		double sum = 0.0;
		for (int i = 0; i < (int)dataVect.size(); ++i) {
			sum += dataVect[i] * dataVect[i];
		}
		return sqrt(sum / dataVect.size());
	}

	// 	slgeom
	double GetDistanceFromPointToPoint(double x1, double y1, double x2, double y2)
	{
		return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
	}

	/// <summary>Iterative Equation, to get better Phi (Point on Ellipse) for shorter distance form Point to Ellipse
	///      Every point on an Self-Centered Ellipse can be expressed by ellipse-equation feature a, b, and angle phi (phi is the angle)
	///      x = a * cos(phi), y = b * sin(phi) </summary>
	/// <remarks> Requirements:   ar > br  !!!!!</remarks>
	/// <param name="xr">x of Relative Point</param>
	/// <param name="yr">y of Relative Point</param>
	/// <param name="ar">a of Relative-Ellipse's Equation, need to be larger than br</param>
	/// <param name="br">b of Relative-Ellipse's Equation</param>
	/// <param name="phi">old phi of an point on the ellipse that was assumed to be with shortest distance to the Relative Point  [IN] </param>
	/// <return>better phi with ar shorter distance from the RandomPoint to the point on Ellipse (to which point the better phi belongs)</return>
	double IteratePhiForShortestDistanceToEllipse(double xr, double yr, double ar, double br, double phi)
	{
		assert(ar > br);// In this algorithm, "ar > br" is required, otherwise it won't work
		return atan(((ar*ar - br * br) * sin(phi) + abs(yr) * br) / (abs(xr) * ar));
	}

	/// <summary>Transfer RamdomPoint to new CoordinateSystem that is relative to ellipse, with ellipse center be Origin and majorAxis be axis-X</summary>
	/// <param name="theta">angle of ellipse in radians</param>
	void GetPointRelativeToEllipse(int x_RandomPoint, int y_RandomPoint, double& x_RelativePoint, double& y_RelativePoint
		, double x_EllipseCenter, double y_EllipseCenter, double theta)
	{
		x_RelativePoint = abs( (x_RandomPoint - x_EllipseCenter) * cos(theta) + (y_RandomPoint - y_EllipseCenter) * sin(theta) );
		y_RelativePoint = abs( -(x_RandomPoint - x_EllipseCenter) * sin(theta) + (y_RandomPoint - y_EllipseCenter) * cos(theta) );
	}

	double GetShortestDistanceFromPointToEllipse(const cv::Point& randomPoint, const cv::RotatedRect& ellipse)
	{
		double phiShortest = CV_PI / 2.0; // starting point has to be 90 degree, if start from 0, iteration may return two same values due to sin(0) == 0
		double shortestDistance = 999999.0;
		double newDistance = shortestDistance - 2.0;
		double majorAxisLength;
		double minorAxisLength;
		double x_RelativePoint;
		double y_RelativePoint;

		// Make sure a > b, and a is always on X-axis;
		if (ellipse.size.width > ellipse.size.height)
		{
			majorAxisLength = ellipse.size.width / 2.0;
			minorAxisLength = ellipse.size.height / 2.0;
			slopencv::GetPointRelativeToEllipse(randomPoint.x, randomPoint.y, x_RelativePoint, y_RelativePoint, ellipse.center.x, ellipse.center.y, (ellipse.angle) / 180.0 * CV_PI);
		}else {
			// if ellipse.size.width < ellipse.size.height, we need to switch the X/Y-axis Coordinate for RelativePoint
			majorAxisLength = ellipse.size.height / 2.0;
			minorAxisLength = ellipse.size.width / 2.0;
			slopencv::GetPointRelativeToEllipse(randomPoint.x, randomPoint.y, y_RelativePoint, x_RelativePoint, ellipse.center.x, ellipse.center.y, (ellipse.angle) / 180.0 * CV_PI);
		}

		int iterationCount = 0;
		while (shortestDistance - newDistance > 0.01)
		{
			shortestDistance = newDistance;
			phiShortest = slopencv::IteratePhiForShortestDistanceToEllipse(x_RelativePoint, y_RelativePoint, majorAxisLength, minorAxisLength, phiShortest);
			newDistance = slopencv::GetDistanceFromPointToPoint(abs(x_RelativePoint), abs(y_RelativePoint), majorAxisLength * cos(phiShortest), minorAxisLength * sin(phiShortest));
			iterationCount++;
		}

		return shortestDistance;
	}


	double GetExtractedEllipseRootMeanSquare(const std::vector<cv::Point>& ellipseEdges, const cv::RotatedRect& bestFitEllipse)
	{
		if (ellipseEdges.size() <= 0)
			return -1.0;

		std::vector<double> edgesDistancesToBestFitEllipse;
		edgesDistancesToBestFitEllipse.resize(ellipseEdges.size());

		for (int i = 0; i < (int)ellipseEdges.size(); ++i)
		{
			edgesDistancesToBestFitEllipse[i] = GetShortestDistanceFromPointToEllipse(ellipseEdges[i], bestFitEllipse);
			// distance less than one pixel could be taken as 0, such that a perfect ellipse RMS = 0
			if (edgesDistancesToBestFitEllipse[i] < 0.99)
				edgesDistancesToBestFitEllipse[i] = 0;
		}

		return GetRootMeanSquare(edgesDistancesToBestFitEllipse);
	}

	/// <summary>Bad solution to Get cv::Mat from CImage</summary>
	/// <param name="src">CImage [IN]</param>
	/// <param name="dst">cv::Mat [OUT]</param>
	void GetImageMat(CImage& src, cv::Mat& dst)
	{
		assert(!src.IsNull());
		dst.release();

		switch (src.GetBPP())
		{
		case 8:
		{
			dst.create(src.GetHeight(), src.GetWidth(), CV_8UC1);
			for (int row = 0; row < src.GetHeight(); row++)
			{
				for (int col = 0; col < src.GetWidth(); col++)
				{
					dst.at<unsigned char>(row, col) = static_cast<BYTE*>(src.GetPixelAddress(col, row))[0];
				}
			}
			break;
		}

		case 24:
		{
			dst.create(src.GetHeight(), src.GetWidth(), CV_8UC3);
			for (int row = 0; row < src.GetHeight(); row++)
			{
				for (int col = 0; col < src.GetWidth(); col++)
				{
					dst.at<cv::Vec3b>(row, col)[0] = static_cast<BYTE*>(src.GetPixelAddress(col, row))[0];
					dst.at<cv::Vec3b>(row, col)[1] = static_cast<BYTE*>(src.GetPixelAddress(col, row))[1];
					dst.at<cv::Vec3b>(row, col)[2] = static_cast<BYTE*>(src.GetPixelAddress(col, row))[2];
				}
			}

			break;
		}

		case 32:
		{
			dst.create(src.GetHeight(), src.GetWidth(), CV_8UC4);
			for (int row = 0; row < src.GetHeight(); row++)
			{
				for (int col = 0; col < src.GetWidth(); col++)
				{
					dst.at<cv::Vec4b>(row, col)[0] = static_cast<BYTE*>(src.GetPixelAddress(col, row))[0];
					dst.at<cv::Vec4b>(row, col)[1] = static_cast<BYTE*>(src.GetPixelAddress(col, row))[1];
					dst.at<cv::Vec4b>(row, col)[2] = static_cast<BYTE*>(src.GetPixelAddress(col, row))[2];
					dst.at<cv::Vec4b>(row, col)[3] = static_cast<BYTE*>(src.GetPixelAddress(col, row))[3];
				}
			}

			break;
		}

		}


	}



}