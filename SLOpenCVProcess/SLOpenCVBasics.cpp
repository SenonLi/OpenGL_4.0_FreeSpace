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
	double GetDistSquareFromPointToPoint(double x1, double y1, double x2, double y2)
	{
		return ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
	}

	/// <Belongs>slgeom::Point2D</Belongs>
	double GetDistanceFromPointToPoint(double x1, double y1, double x2, double y2)
	{
		return sqrt(GetDistSquareFromPointToPoint(x1, y1, x2, y2));
	}

	/// <summary>Iterative Equation, to get better Phi (Point on Ellipse) for shorter distance form Point to Ellipse
	///      Every point on an Self-Centered Ellipse can be expressed by ellipse-equation feature a, b, and angle phi (phi is the angle)
	///      x = a * cos(phi), y = b * sin(phi) </summary>
	/// <remarks> Requirements:   ar >= br  !!!!!</remarks>
	/// <param name="xr">x of Relative Point</param>
	/// <param name="yr">y of Relative Point</param>
	/// <param name="ar">a of Relative-Ellipse's Equation, semi-Major, need to be larger than br by definition</param>
	/// <param name="br">b of Relative-Ellipse's Equation, semi-Minor</param>
	/// <param name="phi">old phi of an point on the ellipse that was assumed to be with shortest distance to the Relative Point  [IN] </param>
	/// <param name="interationCount">Round of Phi iteration, will be used for assertion in case phi too small to make the algorithm work</param>
	/// <return>better phi with ar shorter distance from the RandomPoint to the point on Ellipse (to which point the better phi belongs)</return>
	/// <remarks> Algorithm Reference: http://www.am.ub.edu/~robert/Documents/ellipse.pdf </remarks>
	/// <Belongs>slgeom::Ellipse2D</Belongs>
	double IteratePhiForShortestDistanceToEllipse(double xr, double yr, double ar, double br, double phi, int interationCount)
	{
		assert(br > 0.0 && ar >= br);// In this algorithm, "ar > br" is required, otherwise it won't work
		assert(xr >= 0.0 && yr >= 0.0); // The relative point must be inside the first quadrant
		// For the first time iteration, phi and yr cannot equal to 0.0 at the same time, otherwise the returned "output phi" == "input phi" == 0.0, and iteration equation will never work
		assert(!(interationCount == 0 && phi == 0.0 && yr == 0.0));

		return atan2( (ar*ar - br * br) * sin(phi) + abs(yr) * br,  abs(xr) * ar );
	}

	/// <summary>Transfer RamdomPoint to new CoordinateSystem that is relative to ellipse, with ellipse center be Origin and majorAxis be axis-X</summary>
	/// <remarks>theta in radian (ellipse's angle from X-Axis to semi-Mejor-Axis); to reduce trigonometric-function's high cost, 
	///                  pass down sinTheta and cosTheta directly, in case many points with same theta (same ellipse)</remarks>
	/// <param name="sinTheta">sin(theta) [IN]</param>
	/// <param name="sinTheta">cos(theta) [IN]</param>
	/// <remarks> Translate + Rotation (anti-angle rotation, which means rotate -theta ) </remarks>
	/// <Belongs>slgeom::Ellipse2D</Belongs>
	void GetPointRelativeToEllipse(const cv::Point2d& randomPoint, const cv::Point2d& targetEllipseCenter, double sinTheta, double cosTheta, cv::Point2d& relativePoint)
	{
		relativePoint.x = abs( (randomPoint.x - targetEllipseCenter.x) * cosTheta + (randomPoint.y - targetEllipseCenter.y) * sinTheta );
		relativePoint.y = abs( -(randomPoint.x - targetEllipseCenter.x) * sinTheta + (randomPoint.y - targetEllipseCenter.y) * cosTheta );
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
	double GetDistanceFromPointToEllipse(const cv::Point& randomPoint, const cv::RotatedRect& ellipse, double iterativeCriterion)
	{
		double shortestDistanceSquare = slopencv::MAX_POSITION * slopencv::MAX_POSITION;
		double newDistanceSquare = shortestDistanceSquare - 10.0;// just to make sure the initial value of distance is shorter than shortestDistanceSquare
		double semiMajor, semiMinor;
		if (ellipse.size.width >= ellipse.size.height)
		{
			semiMajor = ellipse.size.width / 2.0;
			semiMinor = ellipse.size.height / 2.0;
		}else {
			semiMajor = ellipse.size.height / 2.0;
			semiMinor = ellipse.size.width / 2.0;
		}
		double thetaInRadian = ellipse.angle / 180.0 * CV_PI;
		double sinTheta = sin(thetaInRadian);
		double cosTheta = cos(thetaInRadian);

		cv::Point2d relativePoint;
		slopencv::GetPointRelativeToEllipse(randomPoint, ellipse.center, sinTheta, cosTheta, relativePoint);
		if (ellipse.size.width < ellipse.size.height)
			relativePoint = { relativePoint.y, relativePoint.x };
		// phiShortest (phi) here is the angle start from semi-Major-Axis of random ellipse, to the intersection point ray 
		// and the ray starts from center of ellipse to the intersection point on elllipse, which is the closest point to the random point on the ellipse
		double phiShortest = atan2(relativePoint.y, relativePoint.x);
		// Make sure the initial phiShortest is not too small, in case slopencv::IteratePhiForShortestDistanceToEllipse won't work well (shortestDistanceSquare would be to close to newDistanceSquare )
		if (phiShortest < POINT_TO_ELLIPSE_INTERATIVE_MIN_PHI_IN_PIXEL)
			phiShortest = POINT_TO_ELLIPSE_INTERATIVE_MIN_PHI_IN_PIXEL;

		int iterationCount = 0;
		while (shortestDistanceSquare - newDistanceSquare > iterativeCriterion)
		{
			shortestDistanceSquare = newDistanceSquare;
			phiShortest = slopencv::IteratePhiForShortestDistanceToEllipse(relativePoint.x, relativePoint.y, semiMajor, semiMinor, phiShortest, iterationCount);
			newDistanceSquare = slopencv::GetDistSquareFromPointToPoint(abs(relativePoint.x), abs(relativePoint.y), semiMajor * cos(phiShortest), semiMinor * sin(phiShortest));
			iterationCount++;
		}
		std::cout << "Shortest Distance : \t " << sqrt(newDistanceSquare) << " , \t Iteration Count : \t " << iterationCount << "\t Times !!\n";

		return sqrt(newDistanceSquare);
	}

	/// <summary>Calculate multiple distances, from multiple random points to one targetEllipse<summary>
	/// <remarks>A cost-saving function processing multiple points with respect to function GetDistanceFromPointToEllipse(...) </remarks>
	/// <param name="distancesVect">Contains distances of each random points to targetEllipse [OUT]</param>
	/// <Belongs>slgeom::Ellipse2D</Belongs>
	void GetDistancesArrayFromPointsToEllipse(const std::vector<cv::Point>& randomPointsVect, const cv::RotatedRect& targetEllipse, double iterativeCriterion, std::vector<double>& distancesVect)
	{
		if (randomPointsVect.empty())	return;

		// Preparation for Relative (to ellipse) Point calculation
		double semiMajor, semiMinor;
		if (targetEllipse.size.width >= targetEllipse.size.height)
		{
			semiMajor = targetEllipse.size.width / 2.0;
			semiMinor = targetEllipse.size.height / 2.0;
		}
		else {
			semiMajor = targetEllipse.size.height / 2.0;
			semiMinor = targetEllipse.size.width / 2.0;
		}
		double thetaInRadian = targetEllipse.angle / 180.0 * CV_PI;
		double sinTheta = sin(thetaInRadian);
		double cosTheta = cos(thetaInRadian);

		// Declarations of varaibles used for distances calculation
		double shortestDistanceSquare, newDistanceSquare;
		double phiShortest;
		cv::Point2d relativePoint;
		distancesVect.resize(randomPointsVect.size());

		for (size_t i = 0; i < randomPointsVect.size(); ++i)
		{
			slopencv::GetPointRelativeToEllipse(randomPointsVect[i], targetEllipse.center, sinTheta, cosTheta, relativePoint);
			if (targetEllipse.size.width < targetEllipse.size.height)
				relativePoint = { relativePoint.y, relativePoint.x };

			shortestDistanceSquare = slopencv::MAX_POSITION * slopencv::MAX_POSITION;
			newDistanceSquare = shortestDistanceSquare - 10.0;// just to make sure the initial value of distance is shorter than shortestDistanceSquare

			// phiShortest (phi) here is the angle start from semi-Major-Axis of random targetEllipse, to the intersection point ray 
			// and the ray starts from center of targetEllipse to the intersection point on elllipse, which is the closest point to the random point on the targetEllipse
			phiShortest = atan2(relativePoint.y, relativePoint.x);
			// Make sure the initial phiShortest is not too small, in case slopencv::IteratePhiForShortestDistanceToEllipse won't work well (shortestDistanceSquare would be to close to newDistanceSquare )
			if (phiShortest < POINT_TO_ELLIPSE_INTERATIVE_MIN_PHI_IN_PIXEL)
				phiShortest = POINT_TO_ELLIPSE_INTERATIVE_MIN_PHI_IN_PIXEL;

			int iterationCount = 0;
			while (shortestDistanceSquare - newDistanceSquare > iterativeCriterion)
			{
				shortestDistanceSquare = newDistanceSquare;
				phiShortest = slopencv::IteratePhiForShortestDistanceToEllipse(relativePoint.x, relativePoint.y, semiMajor, semiMinor, phiShortest, iterationCount);
				newDistanceSquare = slopencv::GetDistSquareFromPointToPoint(abs(relativePoint.x), abs(relativePoint.y), semiMajor * cos(phiShortest), semiMinor * sin(phiShortest));
				iterationCount++;
			}

			distancesVect[i] = sqrt(newDistanceSquare);
		}
	}

	/// <summary>Calculate RMS of a given ellipseEdges with bestFitEllipse, will return -1.0 when the ellipseEdges is empty</summary>
	/// <returns>rms double value, -1.0 means empty ellipseEdges <returns>
	/// <Belongs>slgeom::Ellipse2D</Belongs>
	double GetExtractedEllipseRootMeanSquare(const std::vector<cv::Point>& ellipseEdges, const cv::RotatedRect& bestFitEllipse)
	{
		if (ellipseEdges.empty())
			return INVALID_ELLIPSE_EXTRACTION_RMS;

		std::vector<double> edgesDistancesToBestFitEllipse;
		GetDistancesArrayFromPointsToEllipse(ellipseEdges, bestFitEllipse, POINT_TO_ELLIPSE_INTERATIVE_CRITERION_IN_PIXEL, edgesDistancesToBestFitEllipse);
		for (int i = 0; i < (int)ellipseEdges.size(); ++i)
		{
			// "- 1.0" for every distance for RMS calculation can remove the noise due to pixel quantification
			edgesDistancesToBestFitEllipse[i] = edgesDistancesToBestFitEllipse[i] > 1.0 ? edgesDistancesToBestFitEllipse[i] - 1.0 : 0.0;
			// Will calculate RMS in percentage, with respect to semi-Major a of the ellipse
			edgesDistancesToBestFitEllipse[i] = edgesDistancesToBestFitEllipse[i] * 100.0 / std::max(bestFitEllipse.size.width / 2.0, bestFitEllipse.size.height / 2.0);
		}

		return GetRootMeanSquare(edgesDistancesToBestFitEllipse);
	}

	/// <summary>Bad solution to Get cv::Mat from CImage</summary>
	/// <param name="src">CImage [IN]</param>
	/// <param name="dst">cv::Mat [OUT]</param>
	void ConvertImageToCVMat(CImage& src, cv::Mat& dst)
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