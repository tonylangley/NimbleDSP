/*
Copyright (c) 2014, James Clay

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/


/**
 * @file RealVector.h
 *
 * Definition of the template class RealVector.
 */

#ifndef NimbleDSP_RealVector_h
#define NimbleDSP_RealVector_h

#include "Vector.h"
#include "ComplexVector.h"


namespace NimbleDSP {

/**
 * \brief Vector class for real numbers.
 */
template <class T>
class RealVector : public Vector<T> {
 public:
    /*****************************************************************************************
                                        Constructors
    *****************************************************************************************/
    /**
     * \brief Basic constructor.
     *
     * Just sets the size of \ref buf and the pointer to the scratch buffer, if one is provided.
     * \param size Size of \ref buf.
     * \param scratch Pointer to a scratch buffer.  The scratch buffer can be shared by multiple
     *      objects (in fact, I recommend it), but if there are multiple threads then it should
     *      be shared only by objects that are accessed by a single thread.  Objects in other
     *      threads should have a separate scratch buffer.  If no scratch buffer is provided
     *      then one will be created in methods that require one and destroyed when the method
     *      returns.
     */
    RealVector<T>(unsigned size = DEFAULT_BUF_LEN, std::vector<T> *scratch = NULL) : Vector<T>(size, scratch) {}
    
    /**
     * \brief Vector constructor.
     *
     * Sets buf equal to the input "data" parameter and sets the pointer to the scratch buffer,
     *      if one is provided.
     * \param data Vector that \ref buf will be set equal to.
     * \param scratch Pointer to a scratch buffer.  The scratch buffer can be shared by multiple
     *      objects (in fact, I recommend it), but if there are multiple threads then it should
     *      be shared only by objects that are accessed by a single thread.  Objects in other
     *      threads should have a separate scratch buffer.  If no scratch buffer is provided
     *      then one will be created in methods that require one and destroyed when the method
     *      returns.
     */
    template <typename U>
    RealVector<T>(std::vector<U> data, std::vector<T> *scratch = NULL) : Vector<T>(data, scratch) {}
    
    /**
     * \brief Array constructor.
     *
     * Sets buf equal to the input "data" array and sets the pointer to the scratch buffer,
     *      if one is provided.
     * \param data Array that \ref buf will be set equal to.
     * \param dataLen Length of "data".
     * \param scratch Pointer to a scratch buffer.  The scratch buffer can be shared by multiple
     *      objects (in fact, I recommend it), but if there are multiple threads then it should
     *      be shared only by objects that are accessed by a single thread.  Objects in other
     *      threads should have a separate scratch buffer.  If no scratch buffer is provided
     *      then one will be created in methods that require one and destroyed when the method
     *      returns.
     */
    template <typename U>
    RealVector<T>(U *data, unsigned dataLen, std::vector<T> *scratch = NULL) : Vector<T>(data, dataLen, scratch) {}
    
    /**
     * \brief Copy constructor.
     */
    RealVector<T>(const RealVector<T>& other) {this->vec = other.vec;}
    
    /*****************************************************************************************
                                            Operators
    *****************************************************************************************/
    /**
     * \brief Assignment operator.
     */
    RealVector<T>& operator=(const Vector<T>& rhs) {this->vec = rhs.vec; return *this;}
    
    /*****************************************************************************************
                                             Methods
    *****************************************************************************************/
    /**
     * \brief Sets each element of \ref buf equal to its value to the power of "exponent".
     *
     * \param exponent Exponent to use.
     * \return Reference to "this".
     */
    RealVector<T> & pow(const SLICKDSP_FLOAT_TYPE exponent);
    
    /**
     * \brief Returns the mean (average) of the data in \ref buf.
     */
    const SLICKDSP_FLOAT_TYPE mean() const;
    
    /**
     * \brief Returns the variance of the data in \ref buf.
     */
    const SLICKDSP_FLOAT_TYPE var() const;
    
    /**
     * \brief Returns the standard deviation of the data in \ref buf.
     */
    const SLICKDSP_FLOAT_TYPE stdDev() const {return std::sqrt(this->var());}
    
    /**
     * \brief Returns the median element of \ref buf.
     */
    const T median();
    
    /**
     * \brief Returns the maximum element in \ref buf.
     *
     * \param maxLoc If it isn't equal to NULL the index of the maximum element
     *      will be returned via this pointer.  If more than one element is equal
     *      to the maximum value the index of the first will be returned.
     *      Defaults to NULL.
     */
    const T max(unsigned *maxLoc = NULL) const;
    
    /**
     * \brief Returns the minimum element in \ref buf.
     *
     * \param minLoc If it isn't equal to NULL the index of the minimum element
     *      will be returned via this pointer.  If more than one element is equal
     *      to the minimum value the index of the first will be returned.
     *      Defaults to NULL.
     */
    const T min(unsigned *minLoc = NULL) const;
    
    /**
     * \brief Sets the upper and lower limit of the values in \ref buf.
     *
     * \param val Limiting value for the data in \ref buf.  Any values that
     *      are greater than "val" are made equal to "val", and
     *      any that are less than -val are made equal to -val.
     * \return Reference to "this".
     */
    RealVector<T> & saturate(T val);
    
    /**
     * \brief Convolution method for complex data.
     *
     * \param data The buffer that will be filtered.
     * \param trimTails "False" tells the method to return the entire convolution, which is
     *      the length of "data" plus the length of "this" (the filter) - 1.  "True" tells the
     *      method to retain the size of "data" by trimming the tails at both ends of
     *      the convolution.
     * \return Reference to "data", which holds the result of the convolution.
     */
    virtual ComplexVector<T> & convComplex(ComplexVector<T> & data, bool trimTails);
    
    /**
     * \brief Decimate method for complex data.
     *
     * This method is equivalent to filtering with the \ref conv method and downsampling
     * with the \ref downsample method, but much more efficient.
     *
     * \param data The buffer that will be filtered.
     * \param rate Indicates how much to downsample.
     * \param trimTails "False" tells the method to return the entire convolution.  "True"
     *      tells the method to retain the size of "data" by trimming the tails at both
     *      ends of the convolution.
     * \return Reference to "data", which holds the result of the decimation.
     */
    virtual ComplexVector<T> & decimateComplex(ComplexVector<T> & data, int rate, bool trimTails = false);
    
    /**
     * \brief Interpolation method for complex data.
     *
     * This method is equivalent to upsampling followed by filtering, but is much more efficient.
     *
     * \param data The buffer that will be filtered.
     * \param rate Indicates how much to upsample.
     * \param trimTails "False" tells the method to return the entire convolution.  "True"
     *      tells the method to retain the size of "data" by trimming the tails at both
     *      ends of the convolution.
     * \return Reference to "data", which holds the result of the interpolation.
     */
    virtual ComplexVector<T> & interpComplex(ComplexVector<T> & data, int rate, bool trimTails = false);
    
    /**
     * \brief Resample method for complex data.
     *
     * This method is equivalent to upsampling by "interpRate", filtering, and downsampling
     *      by "decimateRate", but is much more efficient.
     *
     * \param data The buffer that will be filtered.
     * \param interpRate Indicates how much to upsample.
     * \param decimateRate Indicates how much to downsample.
     * \param trimTails "False" tells the method to return the entire convolution.  "True"
     *      tells the method to retain the size of "data" by trimming the tails at both
     *      ends of the convolution.
     * \return Reference to "data", which holds the result of the resampling.
     */
    virtual ComplexVector<T> & resampleComplex(ComplexVector<T> & data, int interpRate, int decimateRate, bool trimTails = false);
};

template <class T>
RealVector<T> & RealVector<T>::pow(const SLICKDSP_FLOAT_TYPE exponent) {
    for (unsigned i=0; i<this->size(); i++) {
        this->vec[i] = (T) std::pow(this->vec[i], exponent);
    }
    return *this;
}

/**
 * \brief Sets each element of "buffer" equal to its value to the power of "exponent".
 *
 * \param buffer The buffer to operate on.
 * \param exponent Exponent to use.
 * \return Reference to "buffer".
 */
template <class T>
RealVector<T> & pow(RealVector<T> & buffer, const SLICKDSP_FLOAT_TYPE exponent) {
    return buffer.pow(exponent);
}

template <class T>
const SLICKDSP_FLOAT_TYPE RealVector<T>::mean() const {
    assert(this->size() > 0);
    SLICKDSP_FLOAT_TYPE sum = 0;
    for (unsigned i=0; i<this->size(); i++) {
        sum += this->vec[i];
    }
    return sum / this->size();
}

/**
 * \brief Returns the mean (average) of the data in "buffer".
 * \param buffer The buffer to operate on.
 */
template <class T>
const SLICKDSP_FLOAT_TYPE mean(RealVector<T> & buffer) {
    return buffer.mean();
}

template <class T>
const SLICKDSP_FLOAT_TYPE RealVector<T>::var() const {
    assert(this->size() > 1);
    SLICKDSP_FLOAT_TYPE meanVal = mean();
    SLICKDSP_FLOAT_TYPE sum = 0;
    for (unsigned i=0; i<this->size(); i++) {
        SLICKDSP_FLOAT_TYPE varDiff = ((SLICKDSP_FLOAT_TYPE) this->vec[i]) - meanVal;
        sum += varDiff * varDiff;
    }
    return sum / (this->size() - 1);
}

/**
 * \brief Returns the variance of the data in "buffer".
 * \param buffer The buffer to operate on.
 */
template <class T>
const SLICKDSP_FLOAT_TYPE var(RealVector<T> & buffer) {
    return buffer.var();
}

/**
 * \brief Returns the standard deviation of the data in "buffer".
 * \param buffer The buffer to operate on.
 */
template <class T>
const SLICKDSP_FLOAT_TYPE stdDev(RealVector<T> & buffer) {
    return buffer.stdDev();
}

template <class T>
const T RealVector<T>::median() {
    assert(this->size() > 0);
    std::vector<T> scratchBuf = this->vec;
    std::sort(scratchBuf.begin(), scratchBuf.end());
    if (this->size() & 1) {
        // Odd number of samples
        return scratchBuf[this->size()/2];
    }
    else {
        // Even number of samples.  Average the two in the middle.
        unsigned topHalfIndex = this->size()/2;
        return (scratchBuf[topHalfIndex] + scratchBuf[topHalfIndex-1]) / ((T) 2);
    }
}

/**
 * \brief Returns the median element of "buffer".
 * \param buffer The buffer to operate on.
 */
template <class T>
const T median(RealVector<T> & buffer) {
    return buffer.median();
}

template <class T>
const T RealVector<T>::max(unsigned *maxLoc) const {
    assert(this->size() > 0);
    T maxVal = this->vec[0];
    unsigned maxIndex = 0;
    
    for (unsigned i=1; i<this->size(); i++) {
        //if (buf[i] > maxVal) {
        if (maxVal < this->vec[i]) {
            maxVal = this->vec[i];
            maxIndex = i;
        }
    }
    if (maxLoc != NULL) {
        *maxLoc = maxIndex;
    }
    return maxVal;
}

/**
 * \brief Returns the maximum element in "buffer".
 *
 * \param buffer The buffer to search.
 * \param maxLoc If it isn't equal to NULL the index of the maximum element
 *      will be returned via this pointer.  If more than one element is equal
 *      to the maximum value the index of the first will be returned.
 *      Defaults to NULL.
 */
template <class T>
const T max(RealVector<T> & buffer, unsigned *maxLoc = NULL) {
    return buffer.max(maxLoc);
}

template <class T>
const T RealVector<T>::min(unsigned *minLoc) const {
    assert(this->size() > 0);
    T minVal = this->vec[0];
    unsigned minIndex = 0;
    
    for (unsigned i=1; i<this->size(); i++) {
        if (this->vec[i] < minVal) {
            minVal = this->vec[i];
            minIndex = i;
        }
    }
    if (minLoc != NULL) {
        *minLoc = minIndex;
    }
    return minVal;
}

/**
 * \brief Returns the minimum element in "buffer".
 *
 * \param buffer The buffer to search.
 * \param minLoc If it isn't equal to NULL the index of the minimum element
 *      will be returned via this pointer.  If more than one element is equal
 *      to the minimum value the index of the first will be returned.
 *      Defaults to NULL.
 */
template <class T>
const T min(RealVector<T> & buffer, unsigned *minLoc = NULL) {
    return buffer.min(minLoc);
}

template <class T>
RealVector<T> & RealVector<T>::saturate(T val) {
    for (unsigned i=0; i<this->size(); i++) {
        if (this->vec[i] > val)
            this->vec[i] = val;
        else if (this->vec[i] < -val)
            this->vec[i] = -val;
    }
    return *this;
}

/**
 * \brief Sets the upper and lower limit of the values in "buffer".
 *
 * \param buffer The buffer to operate on.
 * \param val Limiting value for the data in \ref buf.  Any values that
 *      are greater than "val" are made equal to "val", and
 *      any that are less than -val are made equal to -val.
 * \return Reference to "buffer".
 */
template <class T>
RealVector<T> & saturate(RealVector<T> & buffer, T val) {
    return buffer.saturate(val);
}

template <class T>
ComplexVector<T> & RealVector<T>::convComplex(ComplexVector<T> & data, bool trimTails) {
    int resultIndex;
    int filterIndex;
    int dataIndex;
    std::vector< std::complex<T> > scratch;
    std::vector< std::complex<T> > *dataTmp;
    
    if (data.scratchBuf == NULL) {
        dataTmp = &scratch;
    }
    else {
        dataTmp = data.scratchBuf;
    }
    *dataTmp = data.vec;
    
    if (trimTails) {
        // Initial partial overlap
        int initialTrim = (this->size() - 1) / 2;
        for (resultIndex=0; resultIndex<((int)this->size()-1) - initialTrim; resultIndex++) {
            data[resultIndex] = 0;
            for (dataIndex=0, filterIndex=initialTrim + resultIndex; filterIndex>=0; dataIndex++, filterIndex--) {
                data[resultIndex] += (*dataTmp)[dataIndex] * this->vec[filterIndex];
            }
        }
        
        // Middle full overlap
        for (; resultIndex<(int)dataTmp->size() - initialTrim; resultIndex++) {
            data[resultIndex] = 0;
            for (dataIndex=resultIndex - ((this->size()-1) - initialTrim), filterIndex=this->size()-1;
                 filterIndex>=0; dataIndex++, filterIndex--) {
                data[resultIndex] += (*dataTmp)[dataIndex] * this->vec[filterIndex];
            }
        }

        // Final partial overlap
        for (; resultIndex<(int)data.size(); resultIndex++) {
            data[resultIndex] = 0;
            for (dataIndex=resultIndex - ((this->size()-1) - initialTrim), filterIndex=this->size()-1;
                 dataIndex<(int)dataTmp->size(); dataIndex++, filterIndex--) {
                data[resultIndex] += (*dataTmp)[dataIndex] * this->vec[filterIndex];
            }
        }
    }
    else {
        data.resize(data.size() + this->size() - 1);
        
        // Initial partial overlap
        for (resultIndex=0; resultIndex<(int)this->size()-1; resultIndex++) {
            data[resultIndex] = 0;
            for (dataIndex=0, filterIndex=resultIndex; filterIndex>=0; dataIndex++, filterIndex--) {
                data[resultIndex] += (*dataTmp)[dataIndex] * this->vec[filterIndex];
            }
        }
        
        // Middle full overlap
        for (; resultIndex<(int)dataTmp->size(); resultIndex++) {
            data[resultIndex] = 0;
            for (dataIndex=resultIndex - (this->size()-1), filterIndex=this->size()-1;
                 filterIndex>=0; dataIndex++, filterIndex--) {
                data[resultIndex] += (*dataTmp)[dataIndex] * this->vec[filterIndex];
            }
        }

        // Final partial overlap
        for (; resultIndex<(int)data.size(); resultIndex++) {
            data[resultIndex] = 0;
            for (dataIndex=resultIndex - (this->size()-1), filterIndex=this->size()-1;
                 dataIndex<(int)dataTmp->size(); dataIndex++, filterIndex--) {
                data[resultIndex] += (*dataTmp)[dataIndex] * this->vec[filterIndex];
            }
        }
    }
    return data;
}

/**
 * \brief Convolution function.
 *
 * \param data Buffer to operate on.
 * \param filter The filter that will convolve "data".
 * \param trimTails "False" tells the function to return the entire convolution, which is
 *      the length of "data" plus the length of "filter" - 1.  "True" tells the
 *      function to retain the size of "data" by trimming the tails at both ends of
 *      the convolution.
 * \return Reference to "data", which holds the result of the convolution.
 */
template <class T>
inline ComplexVector<T> & conv(ComplexVector<T> & data, RealVector<T> & filter, bool trimTails = false) {
    return filter.convComplex(data, trimTails);
}

template <class T>
ComplexVector<T> & RealVector<T>::decimateComplex(ComplexVector<T> & data, int rate, bool trimTails) {
    int resultIndex;
    int filterIndex;
    int dataIndex;
    std::vector< std::complex<T> > scratch;
    std::vector< std::complex<T> > *dataTmp;
    
    if (data.scratchBuf == NULL) {
        dataTmp = &scratch;
    }
    else {
        dataTmp = data.scratchBuf;
    }
    *dataTmp = data.vec;
    
    if (trimTails) {
        data.resize((data.size() + rate - 1) / rate);
        
        // Initial partial overlap
        int initialTrim = (this->size() - 1) / 2;
        for (resultIndex=0; resultIndex<(((int)this->size()-1) - initialTrim + rate - 1)/rate; resultIndex++) {
            data[resultIndex] = 0;
            for (dataIndex=0, filterIndex=initialTrim + resultIndex*rate; filterIndex>=0; dataIndex++, filterIndex--) {
                data[resultIndex] += (*dataTmp)[dataIndex] * this->vec[filterIndex];
            }
        }
        
        // Middle full overlap
        for (; resultIndex<((int)dataTmp->size() - initialTrim + rate - 1)/rate; resultIndex++) {
            data[resultIndex] = 0;
            for (dataIndex=resultIndex*rate - ((this->size()-1) - initialTrim), filterIndex=this->size()-1;
                 filterIndex>=0; dataIndex++, filterIndex--) {
                data[resultIndex] += (*dataTmp)[dataIndex] * this->vec[filterIndex];
            }
        }

        // Final partial overlap
        for (; resultIndex<(int)data.size(); resultIndex++) {
            data[resultIndex] = 0;
            for (dataIndex=resultIndex*rate - ((this->size()-1) - initialTrim), filterIndex=this->size()-1;
                 dataIndex<(int)dataTmp->size(); dataIndex++, filterIndex--) {
                data[resultIndex] += (*dataTmp)[dataIndex] * this->vec[filterIndex];
            }
        }
    }
    else {
        data.resize(((data.size() + this->size() - 1) + (rate - 1)) / rate);
        
        // Initial partial overlap
        for (resultIndex=0; resultIndex<((int)this->size()-1+rate-1)/rate; resultIndex++) {
            data[resultIndex] = 0;
            for (dataIndex=0, filterIndex=resultIndex*rate; filterIndex>=0; dataIndex++, filterIndex--) {
                data[resultIndex] += (*dataTmp)[dataIndex] * this->vec[filterIndex];
            }
        }
        
        // Middle full overlap
        for (; resultIndex<((int)dataTmp->size()+rate-1)/rate; resultIndex++) {
            data[resultIndex] = 0;
            for (dataIndex=resultIndex*rate - (this->size()-1), filterIndex=this->size()-1;
                 filterIndex>=0; dataIndex++, filterIndex--) {
                data[resultIndex] += (*dataTmp)[dataIndex] * this->vec[filterIndex];
            }
        }

        // Final partial overlap
        for (; resultIndex<(int)data.size(); resultIndex++) {
            data[resultIndex] = 0;
            for (dataIndex=resultIndex*rate - (this->size()-1), filterIndex=this->size()-1;
                 dataIndex<(int)dataTmp->size(); dataIndex++, filterIndex--) {
                data[resultIndex] += (*dataTmp)[dataIndex] * this->vec[filterIndex];
            }
        }
    }
    return data;
}

/**
 * \brief Decimate function.
 *
 * This function is equivalent to filtering with the \ref conv function and downsampling
 * with the \ref downsample function, but much more efficient.
 *
 * \param data Buffer to operate on.
 * \param rate Indicates how much to downsample.
 * \param filter The filter that will convolve "data".
 * \param trimTails "False" tells the function to return the entire convolution.  "True"
 *      tells the function to retain the size of "data" by trimming the tails at both
 *      ends of the convolution.
 * \return Reference to "data", which holds the result of the decimation.
 */
template <class T>
inline ComplexVector<T> & decimate(ComplexVector<T> & data, int rate, RealVector<T> & filter, bool trimTails = false) {
    return filter.decimateComplex(data, rate, trimTails);
}

template <class T>
ComplexVector<T> & RealVector<T>::interpComplex(ComplexVector<T> & data, int rate, bool trimTails) {
    int resultIndex;
    int filterIndex;
    int dataIndex;
    int dataStart, filterStart;
    std::vector< std::complex<T> > scratch;
    std::vector< std::complex<T> > *dataTmp;
    
    if (data.scratchBuf == NULL) {
        dataTmp = &scratch;
    }
    else {
        dataTmp = data.scratchBuf;
    }
    *dataTmp = data.vec;
    
    if (trimTails) {
        data.resize(data.size() * rate);

        // Initial partial overlap
        int initialTrim = (this->size() - 1) / 2;
        for (resultIndex=0, dataStart=0; resultIndex<(int)this->size()-1 - initialTrim; resultIndex++) {
            data[resultIndex] = 0;
            for (dataIndex=0, filterIndex=initialTrim + resultIndex; filterIndex>=0; dataIndex++, filterIndex-=rate) {
                data[resultIndex] += (*dataTmp)[dataIndex] * this->vec[filterIndex];
            }
        }
       
        // Middle full overlap
        for (dataStart=0, filterStart=(int)this->size()-1; resultIndex<(int)dataTmp->size()*rate - initialTrim; resultIndex++) {
            data[resultIndex] = 0;
            for (dataIndex=dataStart, filterIndex=filterStart;
                 filterIndex>=0; dataIndex++, filterIndex-=rate) {
                data[resultIndex] += (*dataTmp)[dataIndex] * this->vec[filterIndex];
            }
            ++filterStart;
            if (filterStart >= (int)this->size()) {
                // Filter no longer overlaps with this data sample, so the first overlap sample is the next one.  We thus
                // increment the data index and decrement the filter index.
                filterStart -= rate;
                ++dataStart;
            }
        }

        // Final partial overlap
        for (; resultIndex<(int)data.size(); resultIndex++) {
            data[resultIndex] = 0;
            for (dataIndex=dataStart, filterIndex=filterStart;
                 dataIndex<(int)dataTmp->size(); dataIndex++, filterIndex-=rate) {
                data[resultIndex] += (*dataTmp)[dataIndex] * this->vec[filterIndex];
            }
            ++filterStart;
            if (filterStart >= (int)this->size()) {
                // Filter no longer overlaps with this data sample, so the first overlap sample is the next one.  We thus
                // increment the data index and decrement the filter index.
                filterStart -= rate;
                ++dataStart;
            }
        }
    }
    else {
        data.resize(data.size() * rate + this->size() - 1 - (rate - 1));
        
        // Initial partial overlap
        for (resultIndex=0, dataStart=0; resultIndex<(int)this->size()-1; resultIndex++) {
            data[resultIndex] = 0;
            for (dataIndex=0, filterIndex=resultIndex; filterIndex>=0; dataIndex++, filterIndex-=rate) {
                data[resultIndex] += (*dataTmp)[dataIndex] * this->vec[filterIndex];
            }
        }
        
        // Middle full overlap
        for (dataStart=0, filterStart=resultIndex; resultIndex<(int)dataTmp->size()*rate; resultIndex++) {
            data[resultIndex] = 0;
            for (dataIndex=dataStart, filterIndex=filterStart;
                 filterIndex>=0; dataIndex++, filterIndex-=rate) {
                data[resultIndex] += (*dataTmp)[dataIndex] * this->vec[filterIndex];
            }
            ++filterStart;
            if (filterStart >= (int)this->size()) {
                // Filter no longer overlaps with this data sample, so the first overlap sample is the next one.  We thus
                // increment the data index and decrement the filter index.
                filterStart -= rate;
                ++dataStart;
            }
        }

        // Final partial overlap
        for (; resultIndex<(int)data.size(); resultIndex++) {
            data[resultIndex] = 0;
            for (dataIndex=dataStart, filterIndex=filterStart;
                 dataIndex<(int)dataTmp->size(); dataIndex++, filterIndex-=rate) {
                data[resultIndex] += (*dataTmp)[dataIndex] * this->vec[filterIndex];
            }
            ++filterStart;
            if (filterStart >= (int) this->size()) {
                // Filter no longer overlaps with this data sample, so the first overlap sample is the next one.  We thus
                // increment the data index and decrement the filter index.
                filterStart -= rate;
                ++dataStart;
            }
        }
    }
    return data;
}

/**
 * \brief Interpolation function.
 *
 * This function is equivalent to upsampling followed by filtering, but is much more efficient.
 *
 * \param data Buffer to operate on.
 * \param rate Indicates how much to upsample.
 * \param filter The filter that will convolve "data".
 * \param trimTails "False" tells the function to return the entire convolution.  "True"
 *      tells the function to retain the size of "data" by trimming the tails at both
 *      ends of the convolution.
 * \return Reference to "data", which holds the result of the interpolation.
 */
template <class T>
inline ComplexVector<T> & interp(ComplexVector<T> & data, int rate, RealVector<T> & filter, bool trimTails = false) {
    return filter.interpComplex(data, rate, trimTails);
}

template <class T>
ComplexVector<T> & RealVector<T>::resampleComplex(ComplexVector<T> & data, int interpRate, int decimateRate,  bool trimTails) {
    int resultIndex;
    int filterIndex;
    int dataIndex;
    int dataStart, filterStart;
    std::vector< std::complex<T> > scratch;
    std::vector< std::complex<T> > *dataTmp;
    
    if (data.scratchBuf == NULL) {
        dataTmp = &scratch;
    }
    else {
        dataTmp = data.scratchBuf;
    }
    *dataTmp = data.vec;
    
    if (trimTails) {
        int interpLen = data.size() * interpRate;
        int resampLen = (interpLen + decimateRate - 1) / decimateRate;
        data.resize(resampLen);

        // Initial partial overlap
        int initialTrim = (this->size() - 1) / 2;
        for (resultIndex=0, dataStart=0, filterStart=initialTrim;
             resultIndex<((int)this->size()-1 - initialTrim + decimateRate-1)/decimateRate; resultIndex++) {
            data[resultIndex] = 0;
            for (dataIndex=0, filterIndex=filterStart; filterIndex>=0; dataIndex++, filterIndex-=interpRate) {
                data[resultIndex] += (*dataTmp)[dataIndex] * this->vec[filterIndex];
            }
            filterStart += decimateRate;
            while (filterStart >= (int)this->size()) {
                // Filter no longer overlaps with this data sample, so the first overlap sample is the next one.  We thus
                // increment the data index and decrement the filter index.
                filterStart -= interpRate;
                ++dataStart;
            }
        }
        
        // Middle full overlap
        for (; resultIndex<((int)dataTmp->size()*interpRate - initialTrim + decimateRate-1)/decimateRate; resultIndex++) {
            data[resultIndex] = 0;
            for (dataIndex=dataStart, filterIndex=filterStart;
                 filterIndex>=0; dataIndex++, filterIndex-=interpRate) {
                data[resultIndex] += (*dataTmp)[dataIndex] * this->vec[filterIndex];
            }
            filterStart += decimateRate;
            while (filterStart >= (int)this->size()) {
                // Filter no longer overlaps with this data sample, so the first overlap sample is the next one.  We thus
                // increment the data index and decrement the filter index.
                filterStart -= interpRate;
                ++dataStart;
            }
        }
        
        // Final partial overlap
        for (; resultIndex<(int)data.size(); resultIndex++) {
            data[resultIndex] = 0;
            for (dataIndex=dataStart, filterIndex=filterStart;
                 dataIndex<(int)dataTmp->size(); dataIndex++, filterIndex-=interpRate) {
                data[resultIndex] += (*dataTmp)[dataIndex] * this->vec[filterIndex];
            }
            filterStart += decimateRate;
            while (filterStart >= (int)this->size()) {
                // Filter no longer overlaps with this data sample, so the first overlap sample is the next one.  We thus
                // increment the data index and decrement the filter index.
                filterStart -= interpRate;
                ++dataStart;
            }
        }
    }
    else {
        int interpLen = data.size() * interpRate + this->size() - 1 - (interpRate - 1);
        int resampLen = (interpLen + decimateRate - 1) / decimateRate;
        data.resize(resampLen);
        
        // Initial partial overlap
        for (resultIndex=0, dataStart=0, filterStart=0; resultIndex<((int)this->size()-1+decimateRate-1)/decimateRate; resultIndex++) {
            data[resultIndex] = 0;
            for (dataIndex=0, filterIndex=filterStart; filterIndex>=0; dataIndex++, filterIndex-=interpRate) {
                data[resultIndex] += (*dataTmp)[dataIndex] * this->vec[filterIndex];
            }
            filterStart += decimateRate;
            while (filterStart >= (int)this->size()) {
                // Filter no longer overlaps with this data sample, so the first overlap sample is the next one.  We thus
                // increment the data index and decrement the filter index.
                filterStart -= interpRate;
                ++dataStart;
            }
        }
        
        // Middle full overlap
        for (; resultIndex<((int)dataTmp->size()*interpRate + decimateRate-1)/decimateRate; resultIndex++) {
            data[resultIndex] = 0;
            for (dataIndex=dataStart, filterIndex=filterStart;
                 filterIndex>=0; dataIndex++, filterIndex-=interpRate) {
                data[resultIndex] += (*dataTmp)[dataIndex] * this->vec[filterIndex];
            }
            filterStart += decimateRate;
            while (filterStart >= (int)this->size()) {
                // Filter no longer overlaps with this data sample, so the first overlap sample is the next one.  We thus
                // increment the data index and decrement the filter index.
                filterStart -= interpRate;
                ++dataStart;
            }
        }
        
        // Final partial overlap
        for (; resultIndex<(int)data.size(); resultIndex++) {
            data[resultIndex] = 0;
            for (dataIndex=dataStart, filterIndex=filterStart;
                 dataIndex<(int)dataTmp->size(); dataIndex++, filterIndex-=interpRate) {
                data[resultIndex] += (*dataTmp)[dataIndex] * this->vec[filterIndex];
            }
            filterStart += decimateRate;
            while (filterStart >= (int)this->size()) {
                // Filter no longer overlaps with this data sample, so the first overlap sample is the next one.  We thus
                // increment the data index and decrement the filter index.
                filterStart -= interpRate;
                ++dataStart;
            }
        }
    }
    return data;
}

/**
 * \brief Resample function.
 *
 * This function is equivalent to upsampling by "interpRate", filtering, and downsampling
 *      by "decimateRate", but is much more efficient.
 *
 * \param data Buffer to operate on.
 * \param interpRate Indicates how much to upsample.
 * \param decimateRate Indicates how much to downsample.
 * \param filter The filter that will convolve "data".
 * \param trimTails "False" tells the function to return the entire convolution.  "True"
 *      tells the function to retain the size of "data" by trimming the tails at both
 *      ends of the convolution.
 * \return Reference to "data", which holds the result of the resampling.
 */
template <class T>
inline ComplexVector<T> & resample(ComplexVector<T> & data, int interpRate, int decimateRate,
            RealVector<T> & filter, bool trimTails = false) {
    return filter.resampleComplex(data, interpRate, decimateRate, trimTails);
}

};

#endif