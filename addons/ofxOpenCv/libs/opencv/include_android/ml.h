/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                        Intel License Agreement
//
// Copyright (C) 2000, Intel Corporation, all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of Intel Corporation may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#ifndef __OPENCV_ML_H__
#define __OPENCV_ML_H__

// disable deprecation warning which appears in VisualStudio 8.0
#if _MSC_VER >= 1400
#pragma warning( disable : 4996 )
#endif

#ifndef SKIP_INCLUDES

  #include "cxcore.h"
  #include <limits.h>

  #if defined WIN32 || defined _WIN32 || defined WIN64 || defined _WIN64
    #include <windows.h>
  #endif

#else // SKIP_INCLUDES

  #if defined WIN32 || defined _WIN32 || defined WIN64 || defined _WIN64
    #define CV_CDECL __cdecl
    #define CV_STDCALL __stdcall
  #else
    #define CV_CDECL
    #define CV_STDCALL
  #endif

  #ifndef CV_EXTERN_C
    #ifdef __cplusplus
      #define CV_EXTERN_C extern "C"
      #define CV_DEFAULT(val) = val
    #else
      #define CV_EXTERN_C
      #define CV_DEFAULT(val)
    #endif
  #endif

  #ifndef CV_EXTERN_C_FUNCPTR
    #ifdef __cplusplus
      #define CV_EXTERN_C_FUNCPTR(x) extern "C" { typedef x; }
    #else
      #define CV_EXTERN_C_FUNCPTR(x) typedef x
    #endif
  #endif

  #ifndef CV_INLINE
    #if defined __cplusplus
      #define CV_INLINE inline
    #elif (defined WIN32 || defined _WIN32 || defined WIN64 || defined _WIN64) && !defined __GNUC__
      #define CV_INLINE __inline
    #else
      #define CV_INLINE static
    #endif
  #endif /* CV_INLINE */

  #if (defined WIN32 || defined _WIN32 || defined WIN64 || defined _WIN64) && defined CVAPI_EXPORTS
    #define CV_EXPORTS __declspec(dllexport)
  #else
    #define CV_EXPORTS
  #endif

  #ifndef CVAPI
    #define CVAPI(rettype) CV_EXTERN_C CV_EXPORTS rettype CV_CDECL
  #endif

#endif // SKIP_INCLUDES


#ifdef __cplusplus

// Apple defines a check() macro somewhere in the debug headers
// that interferes with a method definiton in this header
#undef check

#include "cvinternal.h"

/****************************************************************************************\
*                               Main struct definitions                                  *
\****************************************************************************************/

/* log(2*PI) */
#define CV_LOG2PI (1.8378770664093454835606594728112)

/* columns of <trainData> matrix are training samples */
#define CV_COL_SAMPLE 0

/* rows of <trainData> matrix are training samples */
#define CV_ROW_SAMPLE 1

#define CV_IS_ROW_SAMPLE(flags) ((flags) & CV_ROW_SAMPLE)

struct CvVectors
{
    int type;
    int dims, count;
    CvVectors* next;
    union
    {
        uchar** ptr;
        float** fl;
        double** db;
    } data;
};

#if 0
/* A structure, representing the lattice range of statmodel parameters.
   It is used for optimizing statmodel parameters by cross-validation method.
   The lattice is logarithmic, so <step> must be greater then 1. */
typedef struct CvParamLattice
{
    double min_val;
    double max_val;
    double step;
}
CvParamLattice;

CV_INLINE CvParamLattice cvParamLattice( double min_val, double max_val,
                                         double log_step )
{
    CvParamLattice pl;
    pl.min_val = MIN( min_val, max_val );
    pl.max_val = MAX( min_val, max_val );
    pl.step = MAX( log_step, 1. );
    return pl;
}

CV_INLINE CvParamLattice cvDefaultParamLattice( void )
{
    CvParamLattice pl = {0,0,0};
    return pl;
}
#endif

/* Variable type */
#define CV_VAR_NUMERICAL    0
#define CV_VAR_ORDERED      0
#define CV_VAR_CATEGORICAL  1

#define CV_TYPE_NAME_ML_SVM         "opencv-ml-svm"
#define CV_TYPE_NAME_ML_KNN         "opencv-ml-knn"
#define CV_TYPE_NAME_ML_NBAYES      "opencv-ml-bayesian"
#define CV_TYPE_NAME_ML_EM          "opencv-ml-em"
#define CV_TYPE_NAME_ML_BOOSTING    "opencv-ml-boost-tree"
#define CV_TYPE_NAME_ML_TREE        "opencv-ml-tree"
#define CV_TYPE_NAME_ML_ANN_MLP     "opencv-ml-ann-mlp"
#define CV_TYPE_NAME_ML_CNN         "opencv-ml-cnn"
#define CV_TYPE_NAME_ML_RTREES      "opencv-ml-random-trees"

#define CV_TRAIN_ERROR  0
#define CV_TEST_ERROR   1

class CV_EXPORTS CvStatModel
{
public:
    CvStatModel();
    virtual ~CvStatModel();

    virtual void clear();

    virtual void save( const char* filename, const char* name=0 ) const;
    virtual void load( const char* filename, const char* name=0 );

    virtual void write( CvFileStorage* storage, const char* name ) const;
    virtual void read( CvFileStorage* storage, CvFileNode* node );

protected:
    const char* default_model_name;
};

/****************************************************************************************\
*                                 Normal Bayes Classifier                                *
\****************************************************************************************/

/* The structure, representing the grid range of statmodel parameters.
   It is used for optimizing statmodel accuracy by varying model parameters,
   the accuracy estimate being computed by cross-validation.
   The grid is logarithmic, so <step> must be greater then 1. */

class CvMLData;

struct CV_EXPORTS CvParamGrid
{
    // SVM params type
    enum { SVM_C=0, SVM_GAMMA=1, SVM_P=2, SVM_NU=3, SVM_COEF=4, SVM_DEGREE=5 };

    CvParamGrid()
    {
        min_val = max_val = step = 0;
    }

    CvParamGrid( double _min_val, double _max_val, double log_step )
    {
        min_val = _min_val;
        max_val = _max_val;
        step = log_step;
    }
    //CvParamGrid( int param_id );
    bool check() const;

    double min_val;
    double max_val;
    double step;
};

class CV_EXPORTS CvNormalBayesClassifier : public CvStatModel
{
public:
    CvNormalBayesClassifier();
    virtual ~CvNormalBayesClassifier();

    CvNormalBayesClassifier( const CvMat* _train_data, const CvMat* _responses,
        const CvMat* _var_idx=0, const CvMat* _sample_idx=0 );
    
    virtual bool train( const CvMat* _train_data, const CvMat* _responses,
        const CvMat* _var_idx = 0, const CvMat* _sample_idx=0, bool update=false );
   
    virtual float predict( const CvMat* _samples, CvMat* results=0 ) const;
    virtual void clear();

#ifndef SWIG
    CvNormalBayesClassifier( const cv::Mat& _train_data, const cv::Mat& _responses,
                            const cv::Mat& _var_idx=cv::Mat(), const cv::Mat& _sample_idx=cv::Mat() );
    virtual bool train( const cv::Mat& _train_data, const cv::Mat& _responses,
                       const cv::Mat& _var_idx = cv::Mat(), const cv::Mat& _sample_idx=cv::Mat(),
                       bool update=false );
    virtual float predict( const cv::Mat& _samples, cv::Mat* results=0 ) const;
#endif
    
    virtual void write( CvFileStorage* storage, const char* name ) const;
    virtual void read( CvFileStorage* storage, CvFileNode* node );

protected:
    int     var_count, var_all;
    CvMat*  var_idx;
    CvMat*  cls_labels;
    CvMat** count;
    CvMat** sum;
    CvMat** productsum;
    CvMat** avg;
    CvMat** inv_eigen_values;
    CvMat** cov_rotate_mats;
    CvMat*  c;
};


/****************************************************************************************\
*                          K-Nearest Neighbour Classifier                                *
\****************************************************************************************/

// k Nearest Neighbors
class CV_EXPORTS CvKNearest : public CvStatModel
{
public:

    CvKNearest();
    virtual ~CvKNearest();

    CvKNearest( const CvMat* _train_data, const CvMat* _responses,
                const CvMat* _sample_idx=0, bool _is_regression=false, int max_k=32 );

    virtual bool train( const CvMat* _train_data, const CvMat* _responses,
                        const CvMat* _sample_idx=0, bool is_regression=false,
                        int _max_k=32, bool _update_base=false );
    
    virtual float find_nearest( const CvMat* _samples, int k, CvMat* results=0,
        const float** neighbors=0, CvMat* neighbor_responses=0, CvMat* dist=0 ) const;
    
#ifndef SWIG
    CvKNearest( const cv::Mat& _train_data, const cv::Mat& _responses,
               const cv::Mat& _sample_idx=cv::Mat(), bool _is_regression=false, int max_k=32 );
    
    virtual bool train( const cv::Mat& _train_data, const cv::Mat& _responses,
                       const cv::Mat& _sample_idx=cv::Mat(), bool is_regression=false,
                       int _max_k=32, bool _update_base=false );    
    
    virtual float find_nearest( const cv::Mat& _samples, int k, cv::Mat* results=0,
                                const float** neighbors=0,
                                cv::Mat* neighbor_responses=0,
                                cv::Mat* dist=0 ) const;
#endif
    
    virtual void clear();
    int get_max_k() const;
    int get_var_count() const;
    int get_sample_count() const;
    bool is_regression() const;

protected:

    virtual float write_results( int k, int k1, int start, int end,
        const float* neighbor_responses, const float* dist, CvMat* _results,
        CvMat* _neighbor_responses, CvMat* _dist, Cv32suf* sort_buf ) const;

    virtual void find_neighbors_direct( const CvMat* _samples, int k, int start, int end,
        float* neighbor_responses, const float** neighbors, float* dist ) const;


    int max_k, var_count;
    int total;
    bool regression;
    CvVectors* samples;
};

/****************************************************************************************\
*                                   Support Vector Machines                              *
\****************************************************************************************/

// SVM training parameters
struct CV_EXPORTS CvSVMParams
{
    CvSVMParams();
    CvSVMParams( int _svm_type, int _kernel_type,
                 double _degree, double _gamma, double _coef0,
                 double Cvalue, double _nu, double _p,
                 CvMat* _class_weights, CvTermCriteria _term_crit );

    int         svm_type;
    int         kernel_type;
    double      degree; // for poly
    double      gamma;  // for poly/rbf/sigmoid
    double      coef0;  // for poly/sigmoid

    double      C;  // for CV_SVM_C_SVC, CV_SVM_EPS_SVR and CV_SVM_NU_SVR
    double      nu; // for CV_SVM_NU_SVC, CV_SVM_ONE_CLASS, and CV_SVM_NU_SVR
    double      p; // for CV_SVM_EPS_SVR
    CvMat*      class_weights; // for CV_SVM_C_SVC
    CvTermCriteria term_crit; // termination criteria
};


struct CV_EXPORTS CvSVMKernel
{
    typedef void (CvSVMKernel::*Calc)( int vec_count, int vec_size, const float** vecs,
                                       const float* another, float* results );
    CvSVMKernel();
    CvSVMKernel( const CvSVMParams* _params, Calc _calc_func );
    virtual bool create( const CvSVMParams* _params, Calc _calc_func );
    virtual ~CvSVMKernel();

    virtual void clear();
    virtual void calc( int vcount, int n, const float** vecs, const float* another, float* results );

    const CvSVMParams* params;
    Calc calc_func;

    virtual void calc_non_rbf_base( int vec_count, int vec_size, const float** vecs,
                                    const float* another, float* results,
                                    double alpha, double beta );

    virtual void calc_linear( int vec_count, int vec_size, const float** vecs,
                              const float* another, float* results );
    virtual void calc_rbf( int vec_count, int vec_size, const float** vecs,
                           const float* another, float* results );
    virtual void calc_poly( int vec_count, int vec_size, const float** vecs,
                            const float* another, float* results );
    virtual void calc_sigmoid( int vec_count, int vec_size, const float** vecs,
                               const float* another, float* results );
};


struct CvSVMKernelRow
{
    CvSVMKernelRow* prev;
    CvSVMKernelRow* next;
    float* data;
};


struct CvSVMSolutionInfo
{
    double obj;
    double rho;
    double upper_bound_p;
    double upper_bound_n;
    double r;   // for Solver_NU
};

class CV_EXPORTS CvSVMSolver
{
public:
    typedef bool (CvSVMSolver::*SelectWorkingSet)( int& i, int& j );
    typedef float* (CvSVMSolver::*GetRow)( int i, float* row, float* dst, bool existed );
    typedef void (CvSVMSolver::*CalcRho)( double& rho, double& r );

    CvSVMSolver();

    CvSVMSolver( int count, int var_count, const float** samples, schar* y,
                 int alpha_count, double* alpha, double Cp, double Cn,
                 CvMemStorage* storage, CvSVMKernel* kernel, GetRow get_row,
                 SelectWorkingSet select_working_set, CalcRho calc_rho );
    virtual bool create( int count, int var_count, const float** samples, schar* y,
                 int alpha_count, double* alpha, double Cp, double Cn,
                 CvMemStorage* storage, CvSVMKernel* kernel, GetRow get_row,
                 SelectWorkingSet select_working_set, CalcRho calc_rho );
    virtual ~CvSVMSolver();

    virtual void clear();
    virtual bool solve_generic( CvSVMSolutionInfo& si );

    virtual bool solve_c_svc( int count, int var_count, const float** samples, schar* y,
                              double Cp, double Cn, CvMemStorage* storage,
                              CvSVMKernel* kernel, double* alpha, CvSVMSolutionInfo& si );
    virtual bool solve_nu_svc( int count, int var_count, const float** samples, schar* y,
                               CvMemStorage* storage, CvSVMKernel* kernel,
                               double* alpha, CvSVMSolutionInfo& si );
    virtual bool solve_one_class( int count, int var_count, const float** samples,
                                  CvMemStorage* storage, CvSVMKernel* kernel,
                                  double* alpha, CvSVMSolutionInfo& si );

    virtual bool solve_eps_svr( int count, int var_count, const float** samples, const float* y,
                                CvMemStorage* storage, CvSVMKernel* kernel,
                                double* alpha, CvSVMSolutionInfo& si );

    virtual bool solve_nu_svr( int count, int var_count, const float** samples, const float* y,
                               CvMemStorage* storage, CvSVMKernel* kernel,
                               double* alpha, CvSVMSolutionInfo& si );

    virtual float* get_row_base( int i, bool* _existed );
    virtual float* get_row( int i, float* dst );

    int sample_count;
    int var_count;
    int cache_size;
    int cache_line_size;
    const float** samples;
    const CvSVMParams* params;
    CvMemStorage* storage;
    CvSVMKernelRow lru_list;
    CvSVMKernelRow* rows;

    int alpha_count;

    double* G;
    double* alpha;

    // -1 - lower bound, 0 - free, 1 - upper bound
    schar* alpha_status;

    schar* y;
    double* b;
    float* buf[2];
    double eps;
    int max_iter;
    double C[2];  // C[0] == Cn, C[1] == Cp
    CvSVMKernel* kernel;

    SelectWorkingSet select_working_set_func;
    CalcRho calc_rho_func;
    GetRow get_row_func;

    virtual bool select_working_set( int& i, int& j );
    virtual bool select_working_set_nu_svm( int& i, int& j );
    virtual void calc_rho( double& rho, double& r );
    virtual void calc_rho_nu_svm( double& rho, double& r );

    virtual float* get_row_svc( int i, float* row, float* dst, bool existed );
    virtual float* get_row_one_class( int i, float* row, float* dst, bool existed );
    virtual float* get_row_svr( int i, float* row, float* dst, bool existed );
};


struct CvSVMDecisionFunc
{
    double rho;
    int sv_count;
    double* alpha;
    int* sv_index;
};


// SVM model
class CV_EXPORTS CvSVM : public CvStatModel
{
public:
    // SVM type
    enum { C_SVC=100, NU_SVC=101, ONE_CLASS=102, EPS_SVR=103, NU_SVR=104 };

    // SVM kernel type
    enum { LINEAR=0, POLY=1, RBF=2, SIGMOID=3 };

    // SVM params type
    enum { C=0, GAMMA=1, P=2, NU=3, COEF=4, DEGREE=5 };

    CvSVM();
    virtual ~CvSVM();

    CvSVM( const CvMat* _train_data, const CvMat* _responses,
           const CvMat* _var_idx=0, const CvMat* _sample_idx=0,
           CvSVMParams _params=CvSVMParams() );

    virtual bool train( const CvMat* _train_data, const CvMat* _responses,
                        const CvMat* _var_idx=0, const CvMat* _sample_idx=0,
                        CvSVMParams _params=CvSVMParams() );
    
    virtual bool train_auto( const CvMat* _train_data, const CvMat* _responses,
        const CvMat* _var_idx, const CvMat* _sample_idx, CvSVMParams _params,
        int k_fold = 10,
        CvParamGrid C_grid      = get_default_grid(CvSVM::C),
        CvParamGrid gamma_grid  = get_default_grid(CvSVM::GAMMA),
        CvParamGrid p_grid      = get_default_grid(CvSVM::P),
        CvParamGrid nu_grid     = get_default_grid(CvSVM::NU),
        CvParamGrid coef_grid   = get_default_grid(CvSVM::COEF),
        CvParamGrid degree_grid = get_default_grid(CvSVM::DEGREE) );

    virtual float predict( const CvMat* _sample, bool returnDFVal=false ) const;

#ifndef SWIG
    CvSVM( const cv::Mat& _train_data, const cv::Mat& _responses,
          const cv::Mat& _var_idx=cv::Mat(), const cv::Mat& _sample_idx=cv::Mat(),
          CvSVMParams _params=CvSVMParams() );
    
    virtual bool train( const cv::Mat& _train_data, const cv::Mat& _responses,
                       const cv::Mat& _var_idx=cv::Mat(), const cv::Mat& _sample_idx=cv::Mat(),
                       CvSVMParams _params=CvSVMParams() );
    
    virtual bool train_auto( const cv::Mat& _train_data, const cv::Mat& _responses,
                            const cv::Mat& _var_idx, const cv::Mat& _sample_idx, CvSVMParams _params,
                            int k_fold = 10,
                            CvParamGrid C_grid      = get_default_grid(CvSVM::C),
                            CvParamGrid gamma_grid  = get_default_grid(CvSVM::GAMMA),
                            CvParamGrid p_grid      = get_default_grid(CvSVM::P),
                            CvParamGrid nu_grid     = get_default_grid(CvSVM::NU),
                            CvParamGrid coef_grid   = get_default_grid(CvSVM::COEF),
                            CvParamGrid degree_grid = get_default_grid(CvSVM::DEGREE) );
    virtual float predict( const cv::Mat& _sample, bool returnDFVal=false ) const;    
#endif
    
    virtual int get_support_vector_count() const;
    virtual const float* get_support_vector(int i) const;
    virtual CvSVMParams get_params() const { return params; };
    virtual void clear();

    static CvParamGrid get_default_grid( int param_id );

    virtual void write( CvFileStorage* storage, const char* name ) const;
    virtual void read( CvFileStorage* storage, CvFileNode* node );
    int get_var_count() const { return var_idx ? var_idx->cols : var_all; }

protected:

    virtual bool set_params( const CvSVMParams& _params );
    virtual bool train1( int sample_count, int var_count, const float** samples,
                    const void* _responses, double Cp, double Cn,
                    CvMemStorage* _storage, double* alpha, double& rho );
    virtual bool do_train( int svm_type, int sample_count, int var_count, const float** samples,
                    const CvMat* _responses, CvMemStorage* _storage, double* alpha );
    virtual void create_kernel();
    virtual void create_solver();

    virtual float predict( const float* row_sample, int row_len, bool returnDFVal=false ) const;

    virtual void write_params( CvFileStorage* fs ) const;
    virtual void read_params( CvFileStorage* fs, CvFileNode* node );

    CvSVMParams params;
    CvMat* class_labels;
    int var_all;
    float** sv;
    int sv_total;
    CvMat* var_idx;
    CvMat* class_weights;
    CvSVMDecisionFunc* decision_func;
    CvMemStorage* storage;

    CvSVMSolver* solver;
    CvSVMKernel* kernel;
};

/****************************************************************************************\
*                              Expectation - Maximization                                *
\****************************************************************************************/

struct CV_EXPORTS CvEMParams
{
    CvEMParams() : nclusters(10), cov_mat_type(1/*CvEM::COV_MAT_DIAGONAL*/),
        start_step(0/*CvEM::START_AUTO_STEP*/), probs(0), weights(0), means(0), covs(0)
    {
        term_crit=cvTermCriteria( CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 100, FLT_EPSILON );
    }

    CvEMParams( int _nclusters, int _cov_mat_type=1/*CvEM::COV_MAT_DIAGONAL*/,
                int _start_step=0/*CvEM::START_AUTO_STEP*/,
                CvTermCriteria _term_crit=cvTermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 100, FLT_EPSILON),
                const CvMat* _probs=0, const CvMat* _weights=0, const CvMat* _means=0, const CvMat** _covs=0 ) :
                nclusters(_nclusters), cov_mat_type(_cov_mat_type), start_step(_start_step),
                probs(_probs), weights(_weights), means(_means), covs(_covs), term_crit(_term_crit)
    {}

    int nclusters;
    int cov_mat_type;
    int start_step;
    const CvMat* probs;
    const CvMat* weights;
    const CvMat* means;
    const CvMat** covs;
    CvTermCriteria term_crit;
};


class CV_EXPORTS CvEM : public CvStatModel
{
public:
    // Type of covariation matrices
    enum { COV_MAT_SPHERICAL=0, COV_MAT_DIAGONAL=1, COV_MAT_GENERIC=2 };

    // The initial step
    enum { START_E_STEP=1, START_M_STEP=2, START_AUTO_STEP=0 };

    CvEM();
    CvEM( const CvMat* samples, const CvMat* sample_idx=0,
          CvEMParams params=CvEMParams(), CvMat* labels=0 );
    //CvEM (CvEMParams params, CvMat * means, CvMat ** covs, CvMat * weights, CvMat * probs, CvMat * log_weight_div_det, CvMat * inv_eigen_values, CvMat** cov_rotate_mats);

    virtual ~CvEM();

    virtual bool train( const CvMat* samples, const CvMat* sample_idx=0,
                        CvEMParams params=CvEMParams(), CvMat* labels=0 );

    virtual float predict( const CvMat* sample, CvMat* probs ) const;

#ifndef SWIG
    CvEM( const cv::Mat& samples, const cv::Mat& sample_idx=cv::Mat(),
         CvEMParams params=CvEMParams(), cv::Mat* labels=0 );
    
    virtual bool train( const cv::Mat& samples, const cv::Mat& sample_idx=cv::Mat(),
                       CvEMParams params=CvEMParams(), cv::Mat* labels=0 );
    
    virtual float predict( const cv::Mat& sample, cv::Mat* probs ) const;
#endif
    
    virtual void clear();

    int           get_nclusters() const;
    const CvMat*  get_means()     const;
    const CvMat** get_covs()      const;
    const CvMat*  get_weights()   const;
    const CvMat*  get_probs()     const;

    inline double         get_log_likelihood     () const { return log_likelihood;     };
    
//    inline const CvMat *  get_log_weight_div_det () const { return log_weight_div_det; };
//    inline const CvMat *  get_inv_eigen_values   () const { return inv_eigen_values;   };
//    inline const CvMat ** get_cov_rotate_mats    () const { return cov_rotate_mats;    };

protected:

    virtual void set_params( const CvEMParams& params,
                             const CvVectors& train_data );
    virtual void init_em( const CvVectors& train_data );
    virtual double run_em( const CvVectors& train_data );
    virtual void init_auto( const CvVectors& samples );
    virtual void kmeans( const CvVectors& train_data, int nclusters,
                         CvMat* labels, CvTermCriteria criteria,
                         const CvMat* means );
    CvEMParams params;
    double log_likelihood;

    CvMat* means;
    CvMat** covs;
    CvMat* weights;
    CvMat* probs;

    CvMat* log_weight_div_det;
    CvMat* inv_eigen_values;
    CvMat** cov_rotate_mats;
};

/****************************************************************************************\
*                                      Decision Tree                                     *
\****************************************************************************************/\
struct CvPair16u32s
{
    unsigned short* u;
    int* i;
};


#define CV_DTREE_CAT_DIR(idx,subset) \
    (2*((subset[(idx)>>5]&(1 << ((idx) & 31)))==0)-1)

struct CvDTreeSplit
{
    int var_idx;
    int condensed_idx;
    int inversed;
    float quality;
    CvDTreeSplit* next;
    union
    {
        int subset[2];
        struct
        {
            float c;
            int split_point;
        }
        ord;
    };
};


struct CvDTreeNode
{
    int class_idx;
    int Tn;
    double value;

    CvDTreeNode* parent;
    CvDTreeNode* left;
    CvDTreeNode* right;

    CvDTreeSplit* split;

    int sample_count;
    int depth;
    int* num_valid;
    int offset;
    int buf_idx;
    double maxlr;

    // global pruning data
    int complexity;
    double alpha;
    double node_risk, tree_risk, tree_error;

    // cross-validation pruning data
    int* cv_Tn;
    double* cv_node_risk;
    double* cv_node_error;

    int get_num_valid(int vi) { return num_valid ? num_valid[vi] : sample_count; }
    void set_num_valid(int vi, int n) { if( num_valid ) num_valid[vi] = n; }
};


struct CV_EXPORTS CvDTreeParams
{
    int   max_categories;
    int   max_depth;
    int   min_sample_count;
    int   cv_folds;
    bool  use_surrogates;
    bool  use_1se_rule;
    bool  truncate_pruned_tree;
    float regression_accuracy;
    const float* priors;

    CvDTreeParams() : max_categories(10), max_depth(INT_MAX), min_sample_count(10),
        cv_folds(10), use_surrogates(true), use_1se_rule(true),
        truncate_pruned_tree(true), regression_accuracy(0.01f), priors(0)
    {}

    CvDTreeParams( int _max_depth, int _min_sample_count,
                   float _regression_accuracy, bool _use_surrogates,
                   int _max_categories, int _cv_folds,
                   bool _use_1se_rule, bool _truncate_pruned_tree,
                   const float* _priors ) :
        max_categories(_max_categories), max_depth(_max_depth),
        min_sample_count(_min_sample_count), cv_folds (_cv_folds),
        use_surrogates(_use_surrogates), use_1se_rule(_use_1se_rule),
        truncate_pruned_tree(_truncate_pruned_tree),
        regression_accuracy(_regression_accuracy),
        priors(_priors)
    {}
};


struct CV_EXPORTS CvDTreeTrainData
{
    CvDTreeTrainData();
    CvDTreeTrainData( const CvMat* _train_data, int _tflag,
                      const CvMat* _responses, const CvMat* _var_idx=0,
                      const CvMat* _sample_idx=0, const CvMat* _var_type=0,
                      const CvMat* _missing_mask=0,
                      const CvDTreeParams& _params=CvDTreeParams(),
                      bool _shared=false, bool _add_labels=false );
    virtual ~CvDTreeTrainData();

    virtual void set_data( const CvMat* _train_data, int _tflag,
                          const CvMat* _responses, const CvMat* _var_idx=0,
                          const CvMat* _sample_idx=0, const CvMat* _var_type=0,
                          const CvMat* _missing_mask=0,
                          const CvDTreeParams& _params=CvDTreeParams(),
                          bool _shared=false, bool _add_labels=false,
                          bool _update_data=false );
    virtual void do_responses_copy();

    virtual void get_vectors( const CvMat* _subsample_idx,
         float* values, uchar* missing, float* responses, bool get_class_idx=false );

    virtual CvDTreeNode* subsample_data( const CvMat* _subsample_idx );

    virtual void write_params( CvFileStorage* fs ) const;
    virtual void read_params( CvFileStorage* fs, CvFileNode* node );

    // release all the data
    virtual void clear();

    int get_num_classes() const;
    int get_var_type(int vi) const;
    int get_work_var_count() const {return work_var_count;}

    virtual const float* get_ord_responses( CvDTreeNode* n, float* values_buf, int* sample_indices_buf );
    virtual const int* get_class_labels( CvDTreeNode* n, int* labels_buf );
    virtual const int* get_cv_labels( CvDTreeNode* n, int* labels_buf );
    virtual const int* get_sample_indices( CvDTreeNode* n, int* indices_buf );
    virtual const int* get_cat_var_data( CvDTreeNode* n, int vi, int* cat_values_buf );
    virtual void get_ord_var_data( CvDTreeNode* n, int vi, float* ord_values_buf, int* sorted_indices_buf,
                                   const float** ord_values, const int** sorted_indices, int* sample_indices_buf );
    virtual int get_child_buf_idx( CvDTreeNode* n );

    ////////////////////////////////////

    virtual bool set_params( const CvDTreeParams& params );
    virtual CvDTreeNode* new_node( CvDTreeNode* parent, int count,
                                   int storage_idx, int offset );

    virtual CvDTreeSplit* new_split_ord( int vi, float cmp_val,
                int split_point, int inversed, float quality );
    virtual CvDTreeSplit* new_split_cat( int vi, float quality );
    virtual void free_node_data( CvDTreeNode* node );
    virtual void free_train_data();
    virtual void free_node( CvDTreeNode* node );

    int sample_count, var_all, var_count, max_c_count;
    int ord_var_count, cat_var_count, work_var_count;
    bool have_labels, have_priors;
    bool is_classifier;
    int tflag;

    const CvMat* train_data;
    const CvMat* responses;
    CvMat* responses_copy; // used in Boosting

    int buf_count, buf_size;
    bool shared;
    int is_buf_16u;
    
    CvMat* cat_count;
    CvMat* cat_ofs;
    CvMat* cat_map;

    CvMat* counts;
    CvMat* buf;
    CvMat* direction;
    CvMat* split_buf;

    CvMat* var_idx;
    CvMat* var_type; // i-th element =
                     //   k<0  - ordered
                     //   k>=0 - categorical, see k-th element of cat_* arrays
    CvMat* priors;
    CvMat* priors_mult;

    CvDTreeParams params;

    CvMemStorage* tree_storage;
    CvMemStorage* temp_storage;

    CvDTreeNode* data_root;

    CvSet* node_heap;
    CvSet* split_heap;
    CvSet* cv_heap;
    CvSet* nv_heap;

    CvRNG rng;
};

class CvDTree;
class CvForestTree;

namespace cv
{
    struct DTreeBestSplitFinder;
    struct ForestTreeBestSplitFinder;
}

class CV_EXPORTS CvDTree : public CvStatModel
{
public:
    CvDTree();
    virtual ~CvDTree();

    virtual bool train( const CvMat* _train_data, int _tflag,
                        const CvMat* _responses, const CvMat* _var_idx=0,
                        const CvMat* _sample_idx=0, const CvMat* _var_type=0,
                        const CvMat* _missing_mask=0,
                        CvDTreeParams params=CvDTreeParams() );

    virtual bool train( CvMLData* _data, CvDTreeParams _params=CvDTreeParams() );

    virtual float calc_error( CvMLData* _data, int type , std::vector<float> *resp = 0 ); // type in {CV_TRAIN_ERROR, CV_TEST_ERROR}

    virtual bool train( CvDTreeTrainData* _train_data, const CvMat* _subsample_idx );

    virtual CvDTreeNode* predict( const CvMat* _sample, const CvMat* _missing_data_mask=0,
                                  bool preprocessed_input=false ) const;

#ifndef SWIG
    virtual bool train( const cv::Mat& _train_data, int _tflag,
                       const cv::Mat& _responses, const cv::Mat& _var_idx=cv::Mat(),
                       const cv::Mat& _sample_idx=cv::Mat(), const cv::Mat& _var_type=cv::Mat(),
                       const cv::Mat& _missing_mask=cv::Mat(),
                       CvDTreeParams params=CvDTreeParams() );
    
    virtual CvDTreeNode* predict( const cv::Mat& _sample, const cv::Mat& _missing_data_mask=cv::Mat(),
                                  bool preprocessed_input=false ) const;
#endif
    
    virtual const CvMat* get_var_importance();
    virtual void clear();

    virtual void read( CvFileStorage* fs, CvFileNode* node );
    virtual void write( CvFileStorage* fs, const char* name ) const;

    // special read & write methods for trees in the tree ensembles
    virtual void read( CvFileStorage* fs, CvFileNode* node,
                       CvDTreeTrainData* data );
    virtual void write( CvFileStorage* fs ) const;

    const CvDTreeNode* get_root() const;
    int get_pruned_tree_idx() const;
    CvDTreeTrainData* get_data();

protected:
    friend struct cv::DTreeBestSplitFinder;

    virtual bool do_train( const CvMat* _subsample_idx );

    virtual void try_split_node( CvDTreeNode* n );
    virtual void split_node_data( CvDTreeNode* n );
    virtual CvDTreeSplit* find_best_split( CvDTreeNode* n );
    virtual CvDTreeSplit* find_split_ord_class( CvDTreeNode* n, int vi, 
                            float init_quality = 0, CvDTreeSplit* _split = 0, uchar* ext_buf = 0 );
    virtual CvDTreeSplit* find_split_cat_class( CvDTreeNode* n, int vi,
                            float init_quality = 0, CvDTreeSplit* _split = 0, uchar* ext_buf = 0 );
    virtual CvDTreeSplit* find_split_ord_reg( CvDTreeNode* n, int vi, 
                            float init_quality = 0, CvDTreeSplit* _split = 0, uchar* ext_buf = 0 );
    virtual CvDTreeSplit* find_split_cat_reg( CvDTreeNode* n, int vi, 
                            float init_quality = 0, CvDTreeSplit* _split = 0, uchar* ext_buf = 0 );
    virtual CvDTreeSplit* find_surrogate_split_ord( CvDTreeNode* n, int vi, uchar* ext_buf = 0 );
    virtual CvDTreeSplit* find_surrogate_split_cat( CvDTreeNode* n, int vi, uchar* ext_buf = 0 );
    virtual double calc_node_dir( CvDTreeNode* node );
    virtual void complete_node_dir( CvDTreeNode* node );
    virtual void cluster_categories( const int* vectors, int vector_count,
        int var_count, int* sums, int k, int* cluster_labels );

    virtual void calc_node_value( CvDTreeNode* node );

    virtual void prune_cv();
    virtual double update_tree_rnc( int T, int fold );
    virtual int cut_tree( int T, int fold, double min_alpha );
    virtual void free_prune_data(bool cut_tree);
    virtual void free_tree();

    virtual void write_node( CvFileStorage* fs, CvDTreeNode* node ) const;
    virtual void write_split( CvFileStorage* fs, CvDTreeSplit* split ) const;
    virtual CvDTreeNode* read_node( CvFileStorage* fs, CvFileNode* node, CvDTreeNode* parent );
    virtual CvDTreeSplit* read_split( CvFileStorage* fs, CvFileNode* node );
    virtual void write_tree_nodes( CvFileStorage* fs ) const;
    virtual void read_tree_nodes( CvFileStorage* fs, CvFileNode* node );

    CvDTreeNode* root;
    CvMat* var_importance;
    CvDTreeTrainData* data;

public:
    int pruned_tree_idx;
};


/****************************************************************************************\
*                                   Random Trees Classifier                              *
\****************************************************************************************/

class CvRTrees;

class CV_EXPORTS CvForestTree: public CvDTree
{
public:
    CvForestTree();
    virtual ~CvForestTree();

    virtual bool train( CvDTreeTrainData* _train_data, const CvMat* _subsample_idx, CvRTrees* forest );

    virtual int get_var_count() const {return data ? data->var_count : 0;}
    virtual void read( CvFileStorage* fs, CvFileNode* node, CvRTrees* forest, CvDTreeTrainData* _data );

    /* dummy methods to avoid warnings: BEGIN */
    virtual bool train( const CvMat* _train_data, int _tflag,
                        const CvMat* _responses, const CvMat* _var_idx=0,
                        const CvMat* _sample_idx=0, const CvMat* _var_type=0,
                        const CvMat* _missing_mask=0,
                        CvDTreeParams params=CvDTreeParams() );

    virtual bool train( CvDTreeTrainData* _train_data, const CvMat* _subsample_idx );
    virtual void read( CvFileStorage* fs, CvFileNode* node );
    virtual void read( CvFileStorage* fs, CvFileNode* node,
                       CvDTreeTrainData* data );
    /* dummy methods to avoid warnings: END */

protected:
    friend struct cv::ForestTreeBestSplitFinder;

    virtual CvDTreeSplit* find_best_split( CvDTreeNode* n );
    CvRTrees* forest;
};


struct CV_EXPORTS CvRTParams : public CvDTreeParams
{
    //Parameters for the forest
    bool calc_var_importance; // true <=> RF processes variable importance
    int nactive_vars;
    CvTermCriteria term_crit;

    CvRTParams() : CvDTreeParams( 5, 10, 0, false, 10, 0, false, false, 0 ),
        calc_var_importance(false), nactive_vars(0)
    {
        term_crit = cvTermCriteria( CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 50, 0.1 );
    }

    CvRTParams( int _max_depth, int _min_sample_count,
                float _regression_accuracy, bool _use_surrogates,
                int _max_categories, const float* _priors, bool _calc_var_importance,
                int _nactive_vars, int max_num_of_trees_in_the_forest,
                float forest_accuracy, int termcrit_type ) :
        CvDTreeParams( _max_depth, _min_sample_count, _regression_accuracy,
                       _use_surrogates, _max_categories, 0,
                       false, false, _priors ),
        calc_var_importance(_calc_var_importance),
        nactive_vars(_nactive_vars)
    {
        term_crit = cvTermCriteria(termcrit_type,
            max_num_of_trees_in_the_forest, forest_accuracy);
    }
};


class CV_EXPORTS CvRTrees : public CvStatModel
{
public:
    CvRTrees();
    virtual ~CvRTrees();
    virtual bool train( const CvMat* _train_data, int _tflag,
                        const CvMat* _responses, const CvMat* _var_idx=0,
                        const CvMat* _sample_idx=0, const CvMat* _var_type=0,
                        const CvMat* _missing_mask=0,
                        CvRTParams params=CvRTParams() );
    
    virtual bool train( CvMLData* data, CvRTParams params=CvRTParams() );
    virtual float predict( const CvMat* sample, const CvMat* missing = 0 ) const;
    virtual float predict_prob( const CvMat* sample, const CvMat* missing = 0 ) const;

#ifndef SWIG
    virtual bool train( const cv::Mat& _train_data, int _tflag,
                       const cv::Mat& _responses, const cv::Mat& _var_idx=cv::Mat(),
                       const cv::Mat& _sample_idx=cv::Mat(), const cv::Mat& _var_type=cv::Mat(),
                       const cv::Mat& _missing_mask=cv::Mat(),
                       CvRTParams params=CvRTParams() );
    virtual float predict( const cv::Mat& sample, const cv::Mat& missing = cv::Mat() ) const;
    virtual float predict_prob( const cv::Mat& sample, const cv::Mat& missing = cv::Mat() ) const;
#endif
    
    virtual void clear();

    virtual const CvMat* get_var_importance();
    virtual float get_proximity( const CvMat* sample1, const CvMat* sample2,
        const CvMat* missing1 = 0, const CvMat* missing2 = 0 ) const;
    
    virtual float calc_error( CvMLData* _data, int type , std::vector<float> *resp = 0 ); // type in {CV_TRAIN_ERROR, CV_TEST_ERROR}

    virtual float get_train_error();    

    virtual void read( CvFileStorage* fs, CvFileNode* node );
    virtual void write( CvFileStorage* fs, const char* name ) const;

    CvMat* get_active_var_mask();
    CvRNG* get_rng();

    int get_tree_count() const;
    CvForestTree* get_tree(int i) const;

protected:

    virtual bool grow_forest( const CvTermCriteria term_crit );

    // array of the trees of the forest
    CvForestTree** trees;
    CvDTreeTrainData* data;
    int ntrees;
    int nclasses;
    double oob_error;
    CvMat* var_importance;
    int nsamples;

    CvRNG rng;
    CvMat* active_var_mask;
};

/****************************************************************************************\
*                           Extremely randomized trees Classifier                        *
\****************************************************************************************/
struct CV_EXPORTS CvERTreeTrainData : public CvDTreeTrainData
{
    virtual void set_data( const CvMat* _train_data, int _tflag,
                          const CvMat* _responses, const CvMat* _var_idx=0,
                          const CvMat* _sample_idx=0, const CvMat* _var_type=0,
                          const CvMat* _missing_mask=0,
                          const CvDTreeParams& _params=CvDTreeParams(),
                          bool _shared=false, bool _add_labels=false,
                          bool _update_data=false );
    virtual void get_ord_var_data( CvDTreeNode* n, int vi, float* ord_values_buf, int* missing_buf,
                                   const float** ord_values, const int** missing, int* sample_buf = 0 );
    virtual const int* get_sample_indices( CvDTreeNode* n, int* indices_buf );
    virtual const int* get_cv_labels( CvDTreeNode* n, int* labels_buf );
    virtual const int* get_cat_var_data( CvDTreeNode* n, int vi, int* cat_values_buf );
    virtual void get_vectors( const CvMat* _subsample_idx, float* values, uchar* missing,
                              float* responses, bool get_class_idx=false );
    virtual CvDTreeNode* subsample_data( const CvMat* _subsample_idx );
    const CvMat* missing_mask;
};

class CV_EXPORTS CvForestERTree : public CvForestTree
{
protected:
    virtual double calc_node_dir( CvDTreeNode* node );
    virtual CvDTreeSplit* find_split_ord_class( CvDTreeNode* n, int vi, 
        float init_quality = 0, CvDTreeSplit* _split = 0, uchar* ext_buf = 0 );
    virtual CvDTreeSplit* find_split_cat_class( CvDTreeNode* n, int vi,
        float init_quality = 0, CvDTreeSplit* _split = 0, uchar* ext_buf = 0 );
    virtual CvDTreeSplit* find_split_ord_reg( CvDTreeNode* n, int vi, 
        float init_quality = 0, CvDTreeSplit* _split = 0, uchar* ext_buf = 0 );
    virtual CvDTreeSplit* find_split_cat_reg( CvDTreeNode* n, int vi, 
        float init_quality = 0, CvDTreeSplit* _split = 0, uchar* ext_buf = 0 );
    virtual void split_node_data( CvDTreeNode* n );
};

class CV_EXPORTS CvERTrees : public CvRTrees
{
public:
    CvERTrees();
    virtual ~CvERTrees();
    virtual bool train( const CvMat* _train_data, int _tflag,
                        const CvMat* _responses, const CvMat* _var_idx=0,
                        const CvMat* _sample_idx=0, const CvMat* _var_type=0,
                        const CvMat* _missing_mask=0,
                        CvRTParams params=CvRTParams());
#ifndef SWIG
    virtual bool train( const cv::Mat& _train_data, int _tflag,
                       const cv::Mat& _responses, const cv::Mat& _var_idx=cv::Mat(),
                       const cv::Mat& _sample_idx=cv::Mat(), const cv::Mat& _var_type=cv::Mat(),
                       const cv::Mat& _missing_mask=cv::Mat(),
                       CvRTParams params=CvRTParams());
#endif
    virtual bool train( CvMLData* data, CvRTParams params=CvRTParams() );
protected:
    virtual bool grow_forest( const CvTermCriteria term_crit );
};


/****************************************************************************************\
*                                   Boosted tree classifier                              *
\****************************************************************************************/

struct CV_EXPORTS CvBoostParams : public CvDTreeParams
{
    int boost_type;
    int weak_count;
    int split_criteria;
    double weight_trim_rate;

    CvBoostParams();
    CvBoostParams( int boost_type, int weak_count, double weight_trim_rate,
                   int max_depth, bool use_surrogates, const float* priors );
};


class CvBoost;

class CV_EXPORTS CvBoostTree: public CvDTree
{
public:
    CvBoostTree();
    virtual ~CvBoostTree();

    virtual bool train( CvDTreeTrainData* _train_data,
                        const CvMat* subsample_idx, CvBoost* ensemble );

    virtual void scale( double s );
    virtual void read( CvFileStorage* fs, CvFileNode* node,
                       CvBoost* ensemble, CvDTreeTrainData* _data );
    virtual void clear();

    /* dummy methods to avoid warnings: BEGIN */
    virtual bool train( const CvMat* _train_data, int _tflag,
                        const CvMat* _responses, const CvMat* _var_idx=0,
                        const CvMat* _sample_idx=0, const CvMat* _var_type=0,
                        const CvMat* _missing_mask=0,
                        CvDTreeParams params=CvDTreeParams() );
    virtual bool train( CvDTreeTrainData* _train_data, const CvMat* _subsample_idx );

    virtual void read( CvFileStorage* fs, CvFileNode* node );
    virtual void read( CvFileStorage* fs, CvFileNode* node,
                       CvDTreeTrainData* data );
    /* dummy methods to avoid warnings: END */

protected:

    virtual void try_split_node( CvDTreeNode* n );
    virtual CvDTreeSplit* find_surrogate_split_ord( CvDTreeNode* n, int vi, uchar* ext_buf = 0 );
    virtual CvDTreeSplit* find_surrogate_split_cat( CvDTreeNode* n, int vi, uchar* ext_buf = 0 );
    virtual CvDTreeSplit* find_split_ord_class( CvDTreeNode* n, int vi, 
        float init_quality = 0, CvDTreeSplit* _split = 0, uchar* ext_buf = 0 );
    virtual CvDTreeSplit* find_split_cat_class( CvDTreeNode* n, int vi,
        float init_quality = 0, CvDTreeSplit* _split = 0, uchar* ext_buf = 0 );
    virtual CvDTreeSplit* find_split_ord_reg( CvDTreeNode* n, int vi, 
        float init_quality = 0, CvDTreeSplit* _split = 0, uchar* ext_buf = 0 );
    virtual CvDTreeSplit* find_split_cat_reg( CvDTreeNode* n, int vi, 
        float init_quality = 0, CvDTreeSplit* _split = 0, uchar* ext_buf = 0 );
    virtual void calc_node_value( CvDTreeNode* n );
    virtual double calc_node_dir( CvDTreeNode* n );

    CvBoost* ensemble;
};


class CV_EXPORTS CvBoost : public CvStatModel
{
public:
    // Boosting type
    enum { DISCRETE=0, REAL=1, LOGIT=2, GENTLE=3 };

    // Splitting criteria
    enum { DEFAULT=0, GINI=1, MISCLASS=3, SQERR=4 };

    CvBoost();
    virtual ~CvBoost();

    CvBoost( const CvMat* _train_data, int _tflag,
             const CvMat* _responses, const CvMat* _var_idx=0,
             const CvMat* _sample_idx=0, const CvMat* _var_type=0,
             const CvMat* _missing_mask=0,
             CvBoostParams params=CvBoostParams() );
    
    virtual bool train( const CvMat* _train_data, int _tflag,
             const CvMat* _responses, const CvMat* _var_idx=0,
             const CvMat* _sample_idx=0, const CvMat* _var_type=0,
             const CvMat* _missing_mask=0,
             CvBoostParams params=CvBoostParams(),
             bool update=false );
    
    virtual bool train( CvMLData* data,
             CvBoostParams params=CvBoostParams(),
             bool update=false );

    virtual float predict( const CvMat* _sample, const CvMat* _missing=0,
                           CvMat* weak_responses=0, CvSlice slice=CV_WHOLE_SEQ,
                           bool raw_mode=false, bool return_sum=false ) const;

#ifndef SWIG
    CvBoost( const cv::Mat& _train_data, int _tflag,
            const cv::Mat& _responses, const cv::Mat& _var_idx=cv::Mat(),
            const cv::Mat& _sample_idx=cv::Mat(), const cv::Mat& _var_type=cv::Mat(),
            const cv::Mat& _missing_mask=cv::Mat(),
            CvBoostParams params=CvBoostParams() );
    
    virtual bool train( const cv::Mat& _train_data, int _tflag,
                       const cv::Mat& _responses, const cv::Mat& _var_idx=cv::Mat(),
                       const cv::Mat& _sample_idx=cv::Mat(), const cv::Mat& _var_type=cv::Mat(),
                       const cv::Mat& _missing_mask=cv::Mat(),
                       CvBoostParams params=CvBoostParams(),
                       bool update=false );
    
    virtual float predict( const cv::Mat& _sample, const cv::Mat& _missing=cv::Mat(),
                          cv::Mat* weak_responses=0, CvSlice slice=CV_WHOLE_SEQ,
                          bool raw_mode=false, bool return_sum=false ) const;
#endif
    
    virtual float calc_error( CvMLData* _data, int type , std::vector<float> *resp = 0 ); // type in {CV_TRAIN_ERROR, CV_TEST_ERROR}

    virtual void prune( CvSlice slice );

    virtual void clear();

    virtual void write( CvFileStorage* storage, const char* name ) const;
    virtual void read( CvFileStorage* storage, CvFileNode* node );
    virtual const CvMat* get_active_vars(bool absolute_idx=true);

    CvSeq* get_weak_predictors();

    CvMat* get_weights();
    CvMat* get_subtree_weights();
    CvMat* get_weak_response();
    const CvBoostParams& get_params() const;
    const CvDTreeTrainData* get_data() const;

protected:

    virtual bool set_params( const CvBoostParams& _params );
    virtual void update_weights( CvBoostTree* tree );
    virtual void trim_weights();
    virtual void write_params( CvFileStorage* fs ) const;
    virtual void read_params( CvFileStorage* fs, CvFileNode* node );

    CvDTreeTrainData* data;
    CvBoostParams params;
    CvSeq* weak;

    CvMat* active_vars;
    CvMat* active_vars_abs;
    bool have_active_cat_vars;

    CvMat* orig_response;
    CvMat* sum_response;
    CvMat* weak_eval;
    CvMat* subsample_mask;
    CvMat* weights;
    CvMat* subtree_weights;
    bool have_subsample;
};


/****************************************************************************************\
*                              Artificial Neural Networks (ANN)                          *
\****************************************************************************************/

/////////////////////////////////// Multi-Layer Perceptrons //////////////////////////////

struct CV_EXPORTS CvANN_MLP_TrainParams
{
    CvANN_MLP_TrainParams();
    CvANN_MLP_TrainParams( CvTermCriteria term_crit, int train_method,
                           double param1, double param2=0 );
    ~CvANN_MLP_TrainParams();

    enum { BACKPROP=0, RPROP=1 };

    CvTermCriteria term_crit;
    int train_method;

    // backpropagation parameters
    double bp_dw_scale, bp_moment_scale;

    // rprop parameters
    double rp_dw0, rp_dw_plus, rp_dw_minus, rp_dw_min, rp_dw_max;
};


class CV_EXPORTS CvANN_MLP : public CvStatModel
{
public:
    CvANN_MLP();
    CvANN_MLP( const CvMat* _layer_sizes,
               int _activ_func=SIGMOID_SYM,
               double _f_param1=0, double _f_param2=0 );

    virtual ~CvANN_MLP();

    virtual void create( const CvMat* _layer_sizes,
                         int _activ_func=SIGMOID_SYM,
                         double _f_param1=0, double _f_param2=0 );
    
    virtual int train( const CvMat* _inputs, const CvMat* _outputs,
                       const CvMat* _sample_weights, const CvMat* _sample_idx=0,
                       CvANN_MLP_TrainParams _params = CvANN_MLP_TrainParams(),
                       int flags=0 );
    virtual float predict( const CvMat* _inputs, CvMat* _outputs ) const;
    
#ifndef SWIG
    CvANN_MLP( const cv::Mat& _layer_sizes,
              int _activ_func=SIGMOID_SYM,
              double _f_param1=0, double _f_param2=0 );
    
    virtual void create( const cv::Mat& _layer_sizes,
                        int _activ_func=SIGMOID_SYM,
                        double _f_param1=0, double _f_param2=0 );    
    
    virtual int train( const cv::Mat& _inputs, const cv::Mat& _outputs,
                      const cv::Mat& _sample_weights, const cv::Mat& _sample_idx=cv::Mat(),
                      CvANN_MLP_TrainParams _params = CvANN_MLP_TrainParams(),
                      int flags=0 );    
    
    virtual float predict( const cv::Mat& _inputs, cv::Mat& _outputs ) const;
#endif
    
    virtual void clear();

    // possible activation functions
    enum { IDENTITY = 0, SIGMOID_SYM = 1, GAUSSIAN = 2 };

    // available training flags
    enum { UPDATE_WEIGHTS = 1, NO_INPUT_SCALE = 2, NO_OUTPUT_SCALE = 4 };

    virtual void read( CvFileStorage* fs, CvFileNode* node );
    virtual void write( CvFileStorage* storage, const char* name ) const;

    int get_layer_count() { return layer_sizes ? layer_sizes->cols : 0; }
    const CvMat* get_layer_sizes() { return layer_sizes; }
    double* get_weights(int layer)
    {
        return layer_sizes && weights &&
            (unsigned)layer <= (unsigned)layer_sizes->cols ? weights[layer] : 0;
    }

protected:

    virtual bool prepare_to_train( const CvMat* _inputs, const CvMat* _outputs,
            const CvMat* _sample_weights, const CvMat* _sample_idx,
            CvVectors* _ivecs, CvVectors* _ovecs, double** _sw, int _flags );

    // sequential random backpropagation
    virtual int train_backprop( CvVectors _ivecs, CvVectors _ovecs, const double* _sw );

    // RPROP algorithm
    virtual int train_rprop( CvVectors _ivecs, CvVectors _ovecs, const double* _sw );

    virtual void calc_activ_func( CvMat* xf, const double* bias ) const;
    virtual void calc_activ_func_deriv( CvMat* xf, CvMat* deriv, const double* bias ) const;
    virtual void set_activ_func( int _activ_func=SIGMOID_SYM,
                                 double _f_param1=0, double _f_param2=0 );
    virtual void init_weights();
    virtual void scale_input( const CvMat* _src, CvMat* _dst ) const;
    virtual void scale_output( const CvMat* _src, CvMat* _dst ) const;
    virtual void calc_input_scale( const CvVectors* vecs, int flags );
    virtual void calc_output_scale( const CvVectors* vecs, int flags );

    virtual void write_params( CvFileStorage* fs ) const;
    virtual void read_params( CvFileStorage* fs, CvFileNode* node );

    CvMat* layer_sizes;
    CvMat* wbuf;
    CvMat* sample_weights;
    double** weights;
    double f_param1, f_param2;
    double min_val, max_val, min_val1, max_val1;
    int activ_func;
    int max_count, max_buf_sz;
    CvANN_MLP_TrainParams params;
    CvRNG rng;
};

#if 0
/****************************************************************************************\
*                            Convolutional Neural Network                                *
\****************************************************************************************/
typedef struct CvCNNLayer CvCNNLayer;
typedef struct CvCNNetwork CvCNNetwork;

#define CV_CNN_LEARN_RATE_DECREASE_HYPERBOLICALLY  1
#define CV_CNN_LEARN_RATE_DECREASE_SQRT_INV        2
#define CV_CNN_LEARN_RATE_DECREASE_LOG_INV         3

#define CV_CNN_GRAD_ESTIM_RANDOM        0
#define CV_CNN_GRAD_ESTIM_BY_WORST_IMG  1

#define ICV_CNN_LAYER                0x55550000
#define ICV_CNN_CONVOLUTION_LAYER    0x00001111
#define ICV_CNN_SUBSAMPLING_LAYER    0x00002222
#define ICV_CNN_FULLCONNECT_LAYER    0x00003333

#define ICV_IS_CNN_LAYER( layer )                                          \
    ( ((layer) != NULL) && ((((CvCNNLayer*)(layer))->flags & CV_MAGIC_MASK)\
        == ICV_CNN_LAYER ))

#define ICV_IS_CNN_CONVOLUTION_LAYER( layer )                              \
    ( (ICV_IS_CNN_LAYER( layer )) && (((CvCNNLayer*) (layer))->flags       \
        & ~CV_MAGIC_MASK) == ICV_CNN_CONVOLUTION_LAYER )

#define ICV_IS_CNN_SUBSAMPLING_LAYER( layer )                              \
    ( (ICV_IS_CNN_LAYER( layer )) && (((CvCNNLayer*) (layer))->flags       \
        & ~CV_MAGIC_MASK) == ICV_CNN_SUBSAMPLING_LAYER )

#define ICV_IS_CNN_FULLCONNECT_LAYER( layer )                              \
    ( (ICV_IS_CNN_LAYER( layer )) && (((CvCNNLayer*) (layer))->flags       \
        & ~CV_MAGIC_MASK) == ICV_CNN_FULLCONNECT_LAYER )

typedef void (CV_CDECL *CvCNNLayerForward)
    ( CvCNNLayer* layer, const CvMat* input, CvMat* output );

typedef void (CV_CDECL *CvCNNLayerBackward)
    ( CvCNNLayer* layer, int t, const CvMat* X, const CvMat* dE_dY, CvMat* dE_dX );

typedef void (CV_CDECL *CvCNNLayerRelease)
    (CvCNNLayer** layer);

typedef void (CV_CDECL *CvCNNetworkAddLayer)
    (CvCNNetwork* network, CvCNNLayer* layer);

typedef void (CV_CDECL *CvCNNetworkRelease)
    (CvCNNetwork** network);

#define CV_CNN_LAYER_FIELDS()           \
    /* Indicator of the layer's type */ \
    int flags;                          \
                                        \
    /* Number of input images */        \
    int n_input_planes;                 \
    /* Height of each input image */    \
    int input_height;                   \
    /* Width of each input image */     \
    int input_width;                    \
                                        \
    /* Number of output images */       \
    int n_output_planes;                \
    /* Height of each output image */   \
    int output_height;                  \
    /* Width of each output image */    \
    int output_width;                   \
                                        \
    /* Learning rate at the first iteration */                      \
    float init_learn_rate;                                          \
    /* Dynamics of learning rate decreasing */                      \
    int learn_rate_decrease_type;                                   \
    /* Trainable weights of the layer (including bias) */           \
    /* i-th row is a set of weights of the i-th output plane */     \
    CvMat* weights;                                                 \
                                                                    \
    CvCNNLayerForward  forward;                                     \
    CvCNNLayerBackward backward;                                    \
    CvCNNLayerRelease  release;                                     \
    /* Pointers to the previous and next layers in the network */   \
    CvCNNLayer* prev_layer;                                         \
    CvCNNLayer* next_layer

typedef struct CvCNNLayer
{
    CV_CNN_LAYER_FIELDS();
}CvCNNLayer;

typedef struct CvCNNConvolutionLayer
{
    CV_CNN_LAYER_FIELDS();
    // Kernel size (height and width) for convolution.
    int K;
    // connections matrix, (i,j)-th element is 1 iff there is a connection between
    // i-th plane of the current layer and j-th plane of the previous layer;
    // (i,j)-th element is equal to 0 otherwise
    CvMat *connect_mask;
    // value of the learning rate for updating weights at the first iteration
}CvCNNConvolutionLayer;

typedef struct CvCNNSubSamplingLayer
{
    CV_CNN_LAYER_FIELDS();
    // ratio between the heights (or widths - ratios are supposed to be equal)
    // of the input and output planes
    int sub_samp_scale;
    // amplitude of sigmoid activation function
    float a;
    // scale parameter of sigmoid activation function
    float s;
    // exp2ssumWX = exp(2<s>*(bias+w*(x1+...+x4))), where x1,...x4 are some elements of X
    // - is the vector used in computing of the activation function in backward
    CvMat* exp2ssumWX;
    // (x1+x2+x3+x4), where x1,...x4 are some elements of X
    // - is the vector used in computing of the activation function in backward
    CvMat* sumX;
}CvCNNSubSamplingLayer;

// Structure of the last layer.
typedef struct CvCNNFullConnectLayer
{
    CV_CNN_LAYER_FIELDS();
    // amplitude of sigmoid activation function
    float a;
    // scale parameter of sigmoid activation function
    float s;
    // exp2ssumWX = exp(2*<s>*(W*X)) - is the vector used in computing of the
    // activation function and it's derivative by the formulae
    // activ.func. = <a>(exp(2<s>WX)-1)/(exp(2<s>WX)+1) == <a> - 2<a>/(<exp2ssumWX> + 1)
    // (activ.func.)' = 4<a><s>exp(2<s>WX)/(exp(2<s>WX)+1)^2
    CvMat* exp2ssumWX;
}CvCNNFullConnectLayer;

typedef struct CvCNNetwork
{
    int n_layers;
    CvCNNLayer* layers;
    CvCNNetworkAddLayer add_layer;
    CvCNNetworkRelease release;
}CvCNNetwork;

typedef struct CvCNNStatModel
{
    CV_STAT_MODEL_FIELDS();
    CvCNNetwork* network;
    // etalons are allocated as rows, the i-th etalon has label cls_labeles[i]
    CvMat* etalons;
    // classes labels
    CvMat* cls_labels;
}CvCNNStatModel;

typedef struct CvCNNStatModelParams
{
    CV_STAT_MODEL_PARAM_FIELDS();
    // network must be created by the functions cvCreateCNNetwork and <add_layer>
    CvCNNetwork* network;
    CvMat* etalons;
    // termination criteria
    int max_iter;
    int start_iter;
    int grad_estim_type;
}CvCNNStatModelParams;

CVAPI(CvCNNLayer*) cvCreateCNNConvolutionLayer(
    int n_input_planes, int input_height, int input_width,
    int n_output_planes, int K,
    float init_learn_rate, int learn_rate_decrease_type,
    CvMat* connect_mask CV_DEFAULT(0), CvMat* weights CV_DEFAULT(0) );

CVAPI(CvCNNLayer*) cvCreateCNNSubSamplingLayer(
    int n_input_planes, int input_height, int input_width,
    int sub_samp_scale, float a, float s,
    float init_learn_rate, int learn_rate_decrease_type, CvMat* weights CV_DEFAULT(0) );

CVAPI(CvCNNLayer*) cvCreateCNNFullConnectLayer(
    int n_inputs, int n_outputs, float a, float s,
    float init_learn_rate, int learning_type, CvMat* weights CV_DEFAULT(0) );

CVAPI(CvCNNetwork*) cvCreateCNNetwork( CvCNNLayer* first_layer );

CVAPI(CvStatModel*) cvTrainCNNClassifier(
            const CvMat* train_data, int tflag,
            const CvMat* responses,
            const CvStatModelParams* params,
            const CvMat* CV_DEFAULT(0),
            const CvMat* sample_idx CV_DEFAULT(0),
            const CvMat* CV_DEFAULT(0), const CvMat* CV_DEFAULT(0) );

/****************************************************************************************\
*                               Estimate classifiers algorithms                          *
\****************************************************************************************/
typedef const CvMat* (CV_CDECL *CvStatModelEstimateGetMat)
                    ( const CvStatModel* estimateModel );

typedef int (CV_CDECL *CvStatModelEstimateNextStep)
                    ( CvStatModel* estimateModel );

typedef void (CV_CDECL *CvStatModelEstimateCheckClassifier)
                    ( CvStatModel* estimateModel,
                const CvStatModel* model,
                const CvMat*       features,
                      int          sample_t_flag,
                const CvMat*       responses );

typedef void (CV_CDECL *CvStatModelEstimateCheckClassifierEasy)
                    ( CvStatModel* estimateModel,
                const CvStatModel* model );

typedef float (CV_CDECL *CvStatModelEstimateGetCurrentResult)
                    ( const CvStatModel* estimateModel,
                            float*       correlation );

typedef void (CV_CDECL *CvStatModelEstimateReset)
                    ( CvStatModel* estimateModel );

//-------------------------------- Cross-validation --------------------------------------
#define CV_CROSS_VALIDATION_ESTIMATE_CLASSIFIER_PARAM_FIELDS()    \
    CV_STAT_MODEL_PARAM_FIELDS();                                 \
    int     k_fold;                                               \
    int     is_regression;                                        \
    CvRNG*  rng

typedef struct CvCrossValidationParams
{
    CV_CROSS_VALIDATION_ESTIMATE_CLASSIFIER_PARAM_FIELDS();
} CvCrossValidationParams;

#define CV_CROSS_VALIDATION_ESTIMATE_CLASSIFIER_FIELDS()    \
    CvStatModelEstimateGetMat               getTrainIdxMat; \
    CvStatModelEstimateGetMat               getCheckIdxMat; \
    CvStatModelEstimateNextStep             nextStep;       \
    CvStatModelEstimateCheckClassifier      check;          \
    CvStatModelEstimateGetCurrentResult     getResult;      \
    CvStatModelEstimateReset                reset;          \
    int     is_regression;                                  \
    int     folds_all;                                      \
    int     samples_all;                                    \
    int*    sampleIdxAll;                                   \
    int*    folds;                                          \
    int     max_fold_size;                                  \
    int         current_fold;                               \
    int         is_checked;                                 \
    CvMat*      sampleIdxTrain;                             \
    CvMat*      sampleIdxEval;                              \
    CvMat*      predict_results;                            \
    int     correct_results;                                \
    int     all_results;                                    \
    double  sq_error;                                       \
    double  sum_correct;                                    \
    double  sum_predict;                                    \
    double  sum_cc;                                         \
    double  sum_pp;                                         \
    double  sum_cp

typedef struct CvCrossValidationModel
{
    CV_STAT_MODEL_FIELDS();
    CV_CROSS_VALIDATION_ESTIMATE_CLASSIFIER_FIELDS();
} CvCrossValidationModel;

CVAPI(CvStatModel*)
cvCreateCrossValidationEstimateModel
           ( int                samples_all,
       const CvStatModelParams* estimateParams CV_DEFAULT(0),
       const CvMat*             sampleIdx CV_DEFAULT(0) );

CVAPI(float)
cvCrossValidation( const CvMat*             trueData,
                         int                tflag,
                   const CvMat*             trueClasses,
                         CvStatModel*     (*createClassifier)( const CvMat*,
                                                                     int,
                                                               const CvMat*,
                                                               const CvStatModelParams*,
                                                               const CvMat*,
                                                               const CvMat*,
                                                               const CvMat*,
                                                               const CvMat* ),
                   const CvStatModelParams* estimateParams CV_DEFAULT(0),
                   const CvStatModelParams* trainParams CV_DEFAULT(0),
                   const CvMat*             compIdx CV_DEFAULT(0),
                   const CvMat*             sampleIdx CV_DEFAULT(0),
                         CvStatModel**      pCrValModel CV_DEFAULT(0),
                   const CvMat*             typeMask CV_DEFAULT(0),
                   const CvMat*             missedMeasurementMask CV_DEFAULT(0) );
#endif

/****************************************************************************************\
*                           Auxilary functions declarations                              *
\****************************************************************************************/

/* Generates <sample> from multivariate normal distribution, where <mean> - is an
   average row vector, <cov> - symmetric covariation matrix */
CVAPI(void) cvRandMVNormal( CvMat* mean, CvMat* cov, CvMat* sample,
                           CvRNG* rng CV_DEFAULT(0) );

/* Generates sample from gaussian mixture distribution */
CVAPI(void) cvRandGaussMixture( CvMat* means[],
                               CvMat* covs[],
                               float weights[],
                               int clsnum,
                               CvMat* sample,
                               CvMat* sampClasses CV_DEFAULT(0) );

#define CV_TS_CONCENTRIC_SPHERES 0

/* creates test set */
CVAPI(void) cvCreateTestSet( int type, CvMat** samples,
                 int num_samples,
                 int num_features,
                 CvMat** responses,
                 int num_classes, ... );


#endif

/****************************************************************************************\
*                                      Data                                             *
\****************************************************************************************/

#include <map>
#include <string>
#include <iostream>

#define CV_COUNT     0
#define CV_PORTION   1

struct CV_EXPORTS CvTrainTestSplit
{
public:
    CvTrainTestSplit();
    CvTrainTestSplit( int _train_sample_count, bool _mix = true);
    CvTrainTestSplit( float _train_sample_portion, bool _mix = true);

    union
    {
        int count;
        float portion;
    } train_sample_part;
    int train_sample_part_mode;

    union
    {
        int *count;
        float *portion;
    } *class_part;
    int class_part_mode;

    bool mix;    
};

class CV_EXPORTS CvMLData
{
public:
    CvMLData();
    virtual ~CvMLData();

    // returns:
    // 0 - OK  
    // 1 - file can not be opened or is not correct
    int read_csv(const char* filename);

    const CvMat* get_values(){ return values; };

    const CvMat* get_responses();

    const CvMat* get_missing(){ return missing; };

    void set_response_idx( int idx ); // old response become predictors, new response_idx = idx
                                      // if idx < 0 there will be no response
    int get_response_idx() { return response_idx; }

    const CvMat* get_train_sample_idx() { return train_sample_idx; };
    const CvMat* get_test_sample_idx() { return test_sample_idx; };
    void mix_train_and_test_idx();
    void set_train_test_split( const CvTrainTestSplit * spl);
    
    const CvMat* get_var_idx();
    void chahge_var_idx( int vi, bool state ); // state == true to set vi-variable as predictor

    const CvMat* get_var_types();
    int get_var_type( int var_idx ) { return var_types->data.ptr[var_idx]; };
    // following 2 methods enable to change vars type
    // use these methods to assign CV_VAR_CATEGORICAL type for categorical variable
    // with numerical labels; in the other cases var types are correctly determined automatically
    void set_var_types( const char* str );  // str examples:
                                            // "ord[0-17],cat[18]", "ord[0,2,4,10-12], cat[1,3,5-9,13,14]",
                                            // "cat", "ord" (all vars are categorical/ordered)
    void change_var_type( int var_idx, int type); // type in { CV_VAR_ORDERED, CV_VAR_CATEGORICAL }    
 
    void set_delimiter( char ch );
    char get_delimiter() { return delimiter; };

    void set_miss_ch( char ch );
    char get_miss_ch() { return miss_ch; };
    
protected:
    virtual void clear();

    void str_to_flt_elem( const char* token, float& flt_elem, int& type);
    void free_train_test_idx();
    
    char delimiter;
    char miss_ch;
    //char flt_separator;

    CvMat* values;
    CvMat* missing;
    CvMat* var_types;
    CvMat* var_idx_mask;

    CvMat* response_out; // header
    CvMat* var_idx_out; // mat
    CvMat* var_types_out; // mat

    int response_idx;

    int train_sample_count;
    bool mix;
   
    int total_class_count;
    std::map<std::string, int> *class_map;

    CvMat* train_sample_idx;
    CvMat* test_sample_idx;
    int* sample_idx; // data of train_sample_idx and test_sample_idx

    CvRNG rng;
};


namespace cv
{
    
typedef CvStatModel StatModel;
typedef CvParamGrid ParamGrid;
typedef CvNormalBayesClassifier NormalBayesClassifier;
typedef CvKNearest KNearest;
typedef CvSVMParams SVMParams;
typedef CvSVMKernel SVMKernel;
typedef CvSVMSolver SVMSolver;
typedef CvSVM SVM;
typedef CvEMParams EMParams;
typedef CvEM ExpectationMaximization;
typedef CvDTreeParams DTreeParams;
typedef CvMLData TrainData;
typedef CvDTree DecisionTree;
typedef CvForestTree ForestTree;
typedef CvRTParams RandomTreeParams;
typedef CvRTrees RandomTrees;
typedef CvERTreeTrainData ERTreeTRainData;
typedef CvForestERTree ERTree;
typedef CvERTrees ERTrees;
typedef CvBoostParams BoostParams;
typedef CvBoostTree BoostTree;
typedef CvBoost Boost;
typedef CvANN_MLP_TrainParams ANN_MLP_TrainParams;
typedef CvANN_MLP NeuralNet_MLP;
    
}

#endif
/* End of file. */
