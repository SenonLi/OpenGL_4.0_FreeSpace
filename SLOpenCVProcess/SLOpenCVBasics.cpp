#include "stdafx.h"
#include "SLOpenCVBasics.h"
#include <codecvt>              // for ws2s() and s2ws
#include <iostream>

namespace slopencv
{
	// slutil, statistic

	/// <Belongs>slutil::slstring::</Belongs>
	std::string ws2s(const std::wstring& wstr)
	{
		using convert_typeX = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_typeX, wchar_t> converterX;

		return converterX.to_bytes(wstr);
	}
	/// <Belongs>slutil::slstring::</Belongs>
	std::wstring s2ws(const std::string& str)
	{
		using convert_typeX = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_typeX, wchar_t> converterX;

		return converterX.from_bytes(str);
	}

	// slutil, statistic

	/// <summary>Calculate RMS of a given Vector, will return -1.0 when the input vector is empty</summary>
	/// <returns>rms double value, -1.0 means empty input vector <returns>
	/// <Belongs>slutil::Math</Belongs>
	double GetRootMeanSquare(const std::vector<double>& dataVect)
	{
		assert(!dataVect.empty());
		if (dataVect.empty())
			return -1.0;

		double sum = 0.0;
		for (int i = 0; i < (int)dataVect.size(); ++i) {
			sum += dataVect[i] * dataVect[i];
		}
		return sqrt(sum / dataVect.size());
	}

	// 	slgeom
	/// <Belongs>slgeom::Point2D</Belongs>
	double GetDistanceFromPointToPoint(double x1, double y1, double x2, double y2)
	{
		return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
	}

	/// <summary>Iterative Equation, to get better Phi (Point on Ellipse) for shorter distance form Point to Ellipse
	///      Every point on an Self-Centered Ellipse can be expressed by ellipse-equation feature a, b, and angle phi (phi is the angle)
	///      x = a * cos(phi), y = b * sin(phi) </summary>
	/// <remarks> Requirements:   ar >= br  !!!!!</remarks>
	/// <param name="xr">x of Relative Point</param>
	/// <param name="yr">y of Relative Point</param>
	/// <param name="ar">a of Relative-Ellipse's Equation, need to be larger than br</param>
	/// <param name="br">b of Relative-Ellipse's Equation</param>
	/// <param name="phi">old phi of an point on the ellipse that was assumed to be with shortest distance to the Relative Point  [IN] </param>
	/// <return>better phi with ar shorter distance from the RandomPoint to the point on Ellipse (to which point the better phi belongs)</return>
	/// <remarks> Algorithm Reference: http://www.am.ub.edu/~robert/Documents/ellipse.pdf </remarks>
	/// <Belongs>slgeom::Ellipse2D</Belongs>
	double IteratePhiForShortestDistanceToEllipse(double xr, double yr, double ar, double br, double phi)
	{
		assert(br > 0.0 && ar >= br);// In this algorithm, "ar > br" is required, otherwise it won't work
		assert(xr >= 0.0 && yr >= 0.0); // The relative point must be inside the first quadrant
		assert(!(phi == 0.0 && yr == 0.0));// phi and yr cannot equal to 0.0 at the same time, otherwise the returned "output phi" == "input phi" == 0.0, and iteration equation will never work

		std::cout << "Input xr = \t" << xr << "\t yr = " << yr;
		std::cout << "\t phi = \t" << phi << std::endl;
		return atan2( (ar*ar - br * br) * sin(phi) + abs(yr) * br,  abs(xr) * ar );
	}

	/// <summary>Transfer RamdomPoint to new CoordinateSystem that is relative to ellipse, with ellipse center be Origin and majorAxis be axis-X</summary>
	/// <remarks>theta in radian (ellipse's angle from X-Axis to semi-Mejor-Axis); to reduce trigonometric-function's high cost, 
	///                  pass down sinTheta and cosTheta directly, in case many points with same theta (same ellipse)</remarks>
	/// <param name="sinTheta">sin(theta) [IN]</param>
	/// <param name="sinTheta">cos(theta) [IN]</param>
	/// <remarks> Algorithm Reference: http://www.am.ub.edu/~robert/Documents/ellipse.pdf </remarks>
	/// <Belongs>slgeom::Ellipse2D</Belongs>
	void GetPointRelativeToEllipse(int x_RandomPoint, int y_RandomPoint, double& x_RelativePoint, double& y_RelativePoint
		, double x_EllipseCenter, double y_EllipseCenter, double sinTheta, double cosTheta)
	{
		x_RelativePoint = abs( (x_RandomPoint - x_EllipseCenter) * cosTheta + (y_RandomPoint - y_EllipseCenter) * sinTheta);
		y_RelativePoint = abs( -(x_RandomPoint - x_EllipseCenter) * sinTheta + (y_RandomPoint - y_EllipseCenter) * cosTheta);
	}

	/// <summary>Calculate distance from point to Ellipse<summary>
	/// <param name="randomPoint"> Regular Point with regular Coordinates, in which CoordSystem ellipse's data will be random.
	///   With respect to relativePoint, whose 2D coordinate is based on a CoordSystem whose origin is Ellipse's center, and X-Axis is the semi-Major Axis 
	///                   in which case, the ellipse will always be in the right center of the relative CoordSystem</summary>
	/// <param name="iterativeCriterion">distance resolution, we use this to do iterative solution for transcendetal equation </summary>
	/// <returns>non-negative distance from point point to Ellipse, not matter inside Ellipse or outside</returns>
	/// <remarks> The returned ShortestDistance value will rarely be zero due to resolution, usually tolerance is needed</remarks>
	/// <remarks> Algorithm Reference: http://www.am.ub.edu/~robert/Documents/ellipse.pdf </remarks>
	/// <Belongs>slgeom::Ellipse2D</Belongs>
	double GetShortestDistanceFromPointToEllipse(const cv::Point& randomPoint, const cv::RotatedRect& ellipse, double iterativeCriterion)
	{
		double shortestDistance = slopencv::MAX_POSITION;
		double newDistance = shortestDistance - 1.0;// just to make sure the initial value of distance is shorter than shortestDistance
		double majorAxisLength, minorAxisLength;
		double x_RelativePoint, y_RelativePoint;
		double thetaInRadian = ellipse.angle / 180.0 * CV_PI;
		double sinTheta = sin(thetaInRadian);
		double cosTheta = cos(thetaInRadian);

		// Make sure a > b, and a is always on X-axis;
		if (ellipse.size.width > ellipse.size.height)
		{
			majorAxisLength = ellipse.size.width / 2.0;
			minorAxisLength = ellipse.size.height / 2.0;
			slopencv::GetPointRelativeToEllipse(randomPoint.x, randomPoint.y, x_RelativePoint, y_RelativePoint, ellipse.center.x, ellipse.center.y, sinTheta, cosTheta);
		}else {
			// if ellipse.size.width < ellipse.size.height, we need to switch the X/Y-axis Coordinate for RelativePoint
			majorAxisLength = ellipse.size.height / 2.0;
			minorAxisLength = ellipse.size.width / 2.0;
			slopencv::GetPointRelativeToEllipse(randomPoint.x, randomPoint.y, y_RelativePoint, x_RelativePoint, ellipse.center.x, ellipse.center.y, sinTheta, cosTheta);
		}

		// phiShortest (phi) here is the angle start from semi-Major-Axis of random ellipse, to the intersection point ray 
		// and the ray starts from center of ellipse to the intersection point on elllipse, which is the closest point to the random point on the ellipse
		double phiShortest = atan2(y_RelativePoint, x_RelativePoint);
		if (phiShortest < POINT_TO_ELLIPSE_INTERATIVE_MIN_PHI_IN_PIXEL)
			phiShortest = POINT_TO_ELLIPSE_INTERATIVE_MIN_PHI_IN_PIXEL; // Make sure the initial phiShortest is not too small, in case  

		int iterationCount = 0;
		while (shortestDistance - newDistance > iterativeCriterion)
		{
			shortestDistance = newDistance;
			phiShortest = slopencv::IteratePhiForShortestDistanceToEllipse(x_RelativePoint, y_RelativePoint, majorAxisLength, minorAxisLength, phiShortest);
			newDistance = slopencv::GetDistanceFromPointToPoint(abs(x_RelativePoint), abs(y_RelativePoint), majorAxisLength * cos(phiShortest), minorAxisLength * sin(phiShortest));
			iterationCount++;
		}
		std::cout << "Shortest Distance : \t " << shortestDistance << " , \t Iteration Count : \t " << iterationCount << "\t Times !!\n";

		return shortestDistance;
	}


	/// <Belongs>slgeom::Ellipse2D</Belongs>
	double GetExtractedEllipseRootMeanSquare(const std::vector<cv::Point>& ellipseEdges, const cv::RotatedRect& bestFitEllipse)
	{
		if (ellipseEdges.size() <= 0)
			return -1.0;

		std::vector<double> edgesDistancesToBestFitEllipse;
		edgesDistancesToBestFitEllipse.resize(ellipseEdges.size());

		for (int i = 0; i < (int)ellipseEdges.size(); ++i)
		{
			edgesDistancesToBestFitEllipse[i] = GetShortestDistanceFromPointToEllipse(ellipseEdges[i], bestFitEllipse, POINT_TO_ELLIPSE_INTERATIVE_CRITERION_IN_PIXEL);
			// "- 1" for every distance for RMS calculation can remove the noise due to pixel quantification
			edgesDistancesToBestFitEllipse[i] = edgesDistancesToBestFitEllipse[i] > 1.0 ? edgesDistancesToBestFitEllipse[i] - 1.0 : 0.0;
			// Will calculate RMS in percentage, with respect to semi-Major a of the ellipse
			edgesDistancesToBestFitEllipse[i] = edgesDistancesToBestFitEllipse[i] * 100.0 / std::max(bestFitEllipse.size.width / 2.0, bestFitEllipse.size.height / 2.0);
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