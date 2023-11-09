#ifndef vtkPVSliceFilter_h
#define vtkPVSliceFilter_h

#include <vtkImageData.h>
#include <vtkSimpleImageToImageFilter.h>

class vtkPVSliceFilter : public vtkSimpleImageToImageFilter
{
public:
    static vtkPVSliceFilter* New();
    vtkTypeMacro(vtkPVSliceFilter, vtkSimpleImageToImageFilter);
    void PrintSelf(ostream& os, vtkIndent indent) override;

    void SetStartPoint(int, int);
    void SetEndPoint(int, int);
protected:
    vtkPVSliceFilter();
    ~vtkPVSliceFilter();

    void SimpleExecute(vtkImageData* in, vtkImageData* out) override;
private:
    vtkPVSliceFilter(const vtkPVSliceFilter&) = delete;
    void operator=(const vtkPVSliceFilter&) = delete;


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
