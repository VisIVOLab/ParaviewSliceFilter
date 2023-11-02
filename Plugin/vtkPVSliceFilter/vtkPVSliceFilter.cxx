#include "vtkPVSliceFilter.h"

#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkObjectFactory.h>
#include <vtkPointData.h>

#include <cmath>

vtkStandardNewMacro(vtkPVSliceFilter);

//----------------------------------------------------------------------------
vtkPVSliceFilter::vtkPVSliceFilter() = default;

//----------------------------------------------------------------------------
vtkPVSliceFilter::~vtkPVSliceFilter() = default;

//----------------------------------------------------------------------------
void vtkPVSliceFilter::PrintSelf(ostream& os, vtkIndent indent)
{
    this->Superclass::PrintSelf(os, indent);
}

int vtkPVSliceFilter::RequestInformation(vtkInformation* vtkNotUsed(request),
                                         vtkInformationVector** vtkNotUsed(inputVector), vtkInformationVector* vtkNotUsed(outputVector))
{
    return 1;
}


int vtkPVSliceFilter::RequestData(vtkInformation *request, vtkInformationVector **inputVector, vtkInformationVector *outputVector)
{
    auto info = inputVector[0]->GetInformationObject(0);
    auto imgData = vtkImageData::SafeDownCast(info->Get(vtkDataObject::DATA_OBJECT()));

    auto outInfo = outputVector->GetInformationObject(0);
    auto outImgData = vtkImageData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

    return extractPVSlice(imgData, outImgData);
}

int vtkPVSliceFilter::extractPVSlice(vtkImageData *in, vtkImageData *out)
{
    auto inDims = in->GetDimensions();
    int xDim = std::ceil(std::sqrt(std::pow(EndPoint.first - StartPoint.first, 2) + std::pow(EndPoint.second - StartPoint.second, 2)));
    int yDim = inDims[2];
    int dims[3] = {xDim, yDim, 1};

    out->SetDimensions(dims);

    double xPixW = ((EndPoint.first - StartPoint.first) * 1.0f) / xDim;
    double yPixW = ((EndPoint.second - StartPoint.second) * 1.0f) / xDim;
    for (int i = 0; i < xDim; ++i){
        for (int j = 0; j < yDim; ++j){
            auto idx = std::make_pair(StartPoint.first + (xPixW * i), StartPoint.second + (yPixW * i));
            double d00, d01, d10, d11;
            d00 = in->GetScalarComponentAsDouble(std::floor(idx.first), std::floor(idx.second), j, 0);
            d01 = in->GetScalarComponentAsDouble(std::floor(idx.first), std::floor(idx.second) + 1, j, 0);
            d10 = in->GetScalarComponentAsDouble(std::floor(idx.first) + 1, std::floor(idx.second), j, 0);
            d11 = in->GetScalarComponentAsDouble(std::floor(idx.first) + 1, std::floor(idx.second) + 1, j, 0);
            double x = idx.first - std::floor(idx.first);
            double y = idx.second - std::floor(idx.second);
            double pixIJ = interpolate(d00, d01, d10, d11, x, y);

            //We're creating 2D image, so z-axis coord is always 0. Writing to 0th component.
            out->SetScalarComponentFromDouble(i, j, 0, 0, pixIJ);
        }
    }

    return 1;
}

void vtkPVSliceFilter::SetStartPoint(int32_t newStartPointX, int32_t newStartPointY)
{
    StartPoint = std::make_pair(newStartPointX, newStartPointY);
}

void vtkPVSliceFilter::SetEndPoint(int32_t newEndPointX, int32_t newEndPointY)
{
    EndPoint = std::make_pair(newEndPointX, newEndPointY);
}
