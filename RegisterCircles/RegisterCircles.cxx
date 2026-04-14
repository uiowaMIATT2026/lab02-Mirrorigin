#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageMomentsCalculator.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <chrono>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <fixedImage> <movingImage>" << std::endl;
        return EXIT_FAILURE;
    }

    constexpr unsigned int Dimension = 2;
    using PixelType = unsigned char;
    using ImageType = itk::Image<PixelType, Dimension>;
    using ReaderType = itk::ImageFileReader<ImageType>;
    using MomentsCalculatorType = itk::ImageMomentsCalculator<ImageType>;

    try {
        auto fixedReader = ReaderType::New();
        fixedReader->SetFileName(argv[1]);
        fixedReader->Update();

        auto movingReader = ReaderType::New();
        movingReader->SetFileName(argv[2]);
        movingReader->Update();

        ImageType::Pointer fixedImage = fixedReader->GetOutput();
        ImageType::Pointer movingImage = movingReader->GetOutput();

        auto start_time = std::chrono::high_resolution_clock::now();

        auto fixedCalculator = MomentsCalculatorType::New();
        fixedCalculator->SetImage(fixedImage);

        auto movingCalculator = MomentsCalculatorType::New();
        movingCalculator->SetImage(movingImage);

        fixedCalculator->Compute();
        movingCalculator->Compute();

        auto centerFixed = fixedCalculator->GetCenterOfGravity();
        auto centerMoving = movingCalculator->GetCenterOfGravity();
        double massFixed = fixedCalculator->GetTotalMass();
        double massMoving = movingCalculator->GetTotalMass();

        double translationX = centerFixed[0] - centerMoving[0];
        double translationY = centerFixed[1] - centerMoving[1];
        double scale = std::sqrt(massFixed / massMoving);

        auto end_time = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();

        std::cout << "--- Registration Completed ---" << std::endl;
        std::cout << std::fixed << std::setprecision(4);
        std::cout << "Translation X : " << translationX << " mm" << std::endl;
        std::cout << "Translation Y : " << translationY << " mm" << std::endl;
        std::cout << "Scale Factor  : " << scale << std::endl;
        std::cout << "Execution Time: " << duration << " microseconds" << std::endl;

    } catch (itk::ExceptionObject & err) {
        std::cerr << "ITK Exception caught!" << std::endl;
        std::cerr << err << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}