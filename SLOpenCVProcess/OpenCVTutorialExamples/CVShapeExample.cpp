#include "../stdafx.h"
#include "CVShapeExample.h"

#include <iostream>

static void help()
{
	printf("\n"
		"This program demonstrates a method for shape comparison based on Shape Context\n"
		"You should run the program providing a number between 1 and 20 for selecting an image in the folder ../data/shape_sample.\n"
		"Call\n"
		"./shape_example [number between 1 and 20, 1 default]\n\n");
}

static std::vector<cv::Point> simpleContour(const cv::Mat& currentQuery, int n = 300)
{
	std::vector<std::vector<cv::Point> > _contoursQuery;
	std::vector <cv::Point> contoursQuery;
	findContours(currentQuery, _contoursQuery, cv::RETR_LIST, cv::CHAIN_APPROX_NONE);
	for (size_t border = 0; border<_contoursQuery.size(); border++)
	{
		for (size_t p = 0; p<_contoursQuery[border].size(); p++)
		{
			contoursQuery.push_back(_contoursQuery[border][p]);
		}
	}

	// In case actual number of points is less than n
	int dummy = 0;
	for (int add = (int)contoursQuery.size() - 1; add<n; add++)
	{
		contoursQuery.push_back(contoursQuery[dummy++]); //adding dummy values
	}

	// Uniformly sampling
	cv::randShuffle(contoursQuery);
	std::vector<cv::Point> cont;
	for (int i = 0; i<n; i++)
	{
		cont.push_back(contoursQuery[i]);
	}
	return cont;
}

void CVShapeExample::ShowWidget(int argc, char *argv[])
{
	std::string path = "../WatchMe/Images/OpenCV_ShapeSample/";
	cv::CommandLineParser parser(argc, argv, "{help h||}{@input|1|}");
	if (parser.has("help"))
	{
		help();
		return;
	}
	int indexQuery = parser.get<int>("@input");
	if (!parser.check())
	{
		parser.printErrors();
		help();
		return;
	}
	if (indexQuery < 1 || indexQuery > 20)
	{
		help();
		return;
	}
	cv::Ptr <cv::ShapeContextDistanceExtractor> mysc = cv::createShapeContextDistanceExtractor();

	cv::Size sz2Sh(300, 300);
	std::stringstream queryName;
	queryName << path << indexQuery << ".png";
	cv::Mat query = cv::imread(queryName.str(), cv::IMREAD_GRAYSCALE);
	cv::Mat queryToShow;
	resize(query, queryToShow, sz2Sh, 0, 0, cv::INTER_LINEAR_EXACT);
	cv::imshow("QUERY", queryToShow);
	cv::moveWindow("TEST", 0, 0);
	std::vector<cv::Point> contQuery = simpleContour(query);
	int bestMatch = 0;
	float bestDis = FLT_MAX;
	for (int ii = 1; ii <= 20; ii++)
	{
		if (ii == indexQuery) continue;
		cv::waitKey(30);
		std::stringstream iiname;
		iiname << path << ii << ".png";
		std::cout << "name: " << iiname.str() << std::endl;
		cv::Mat iiIm = cv::imread(iiname.str(), 0);
		cv::Mat iiToShow;
		cv::resize(iiIm, iiToShow, sz2Sh, 0, 0, cv::INTER_LINEAR_EXACT);
		cv::imshow("TEST", iiToShow);
		cv::moveWindow("TEST", sz2Sh.width + 50, 0);
		std::vector<cv::Point> contii = simpleContour(iiIm);
		float dis = mysc->computeDistance(contQuery, contii);
		if (dis<bestDis)
		{
			bestMatch = ii;
			bestDis = dis;
		}
		std::cout << " distance between " << queryName.str() << " and " << iiname.str() << " is: " << dis << std::endl;
	}
	cv::destroyWindow("TEST");
	std::stringstream bestname;
	bestname << path << bestMatch << ".png";
	cv::Mat iiIm = cv::imread(bestname.str(), 0);
	cv::Mat bestToShow;
	cv::resize(iiIm, bestToShow, sz2Sh, 0, 0, cv::INTER_LINEAR_EXACT);
	cv::imshow("BEST MATCH", bestToShow);
	cv::moveWindow("BEST MATCH", sz2Sh.width + 50, 0);
	cv::waitKey();
}

//int main(int argc, char *argv[])
//{
//	CVShapeExample cameraWidget;
//	cameraWidget.ShowWidget(argc, argv);
//
//	return 1;
//}