#ifndef vtkPVSliceFilter_h
#define vtkPVSliceFilter_h

#include <vtkImageData.h>
#include <vtkAlgorithm.h>

class vtkPVSliceFilter : public vtkAlgorithm
{
public:
    static vtkPVSliceFilter* New();
    vtkTypeMacro(vtkPVSliceFilter, vtkAlgorithm);
    void PrintSelf(ostream& os, vtkIndent indent) override;    

protected:
    vtkPVSliceFilter();
    ~vtkPVSliceFilter();

    int RequestInformation(vtkInformation* request, vtkInformationVector** inInfo, vtkInformationVector* outInfo);
    int RequestData(vtkInformation* request, vtkInformationVector** inputVector, vtkInformationVector* outputVector);

    void SetStartPoint(int32_t, int32_t);
    void SetEndPoint(int32_t, int32_t);
private:
    vtkPVSliceFilter(const vtkPVSliceFilter&) = delete;
    void operator=(const vtkPVSliceFilter&) = delete;

    int extractPVSlice(vtkImageData* in, vtkImageData* out);

    std::pair<int, int> StartPoint;
    std::pair<int, int> EndPoint;
};

/**
 * Linear interpolation
 */
template<typename T>
static const T interpolate(const T& fromPointA, const T& toPointB, float factor){
    return ((1.0 - factor) * fromPointA) + (factor * toPointB);
}

/**
 * Bilinear interpolation
 */
template<typename T>
static const T interpolate(const T& fromPointA1, const T& fromPointA2, const T& toPointB1, const T& toPointB2, float factor1, float factor2){
    return interpolate(interpolate(fromPointA1, fromPointA2, factor1), interpolate(toPointB1, toPointB2, factor1), factor2);
}

#endif // vtkPVSliceFilter_h
