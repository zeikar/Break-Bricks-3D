/*
Authored by Prof. Jeong-Mo Hong, CSE Dongguk University
for Introduction to Computer Graphics, 2017 Spring
*/

#pragma once

#include "DataStructure/VectorND.h"
#include "MATRIXNXN.h"
//#include "../Math_Tools/sqr.h"

#ifndef __sqr__
#define __sqr__
template<class T>
inline T sqr(const T a)
{return a*a;}

#endif

template<class T>
class MATRIX_MXN
{
public:
    int m_,n_; // size of the m by n matrix
    T *x_; // pointer to the one dimensional data
    MATRIX_MXN<T> *Q_,*V_;
    MATRIX_NXN<T> *R_;

    MATRIX_MXN()
        :m_(0),n_(0),x_(0),Q_(0),V_(0),R_(0)
    {}

    MATRIX_MXN(const int m_input,const int n_input)
        :m_(m_input),n_(n_input),Q_(0),V_(0),R_(0)
    {
        x_=new T[m_*n_];
		int k;
        for(k=0;k<m_*n_;k++) 
			x_[k]=0;
    }

    MATRIX_MXN(const MATRIX_MXN<T>& A)
        :m_(A.m_),n_(A.n_),Q_(0),V_(0),R_(0)
    {
        x_=new T[m_*n_];
        for(int k=0;k<m_*n_;k++) x_[k]=A.x_[k];
    }

   ~MATRIX_MXN()
   {if(x_) delete[] x_;if(Q_) delete Q_;if(R_) delete(R_);if(V_) delete(V_);}

    void Resize(const int m_new,const int n_new,const bool copy_exciting_elements=true)
    {
        if(m_new == m_ && n_new == n_) return;
        
        T *x_new = new T[m_new*n_new];
        
        for(int t = 0; t < m_new*n_new; t++) x_new[t]=(T)0;
        
        int m1 = MIN2(m_, m_new), n1 = MIN2(n_, n_new);
        
        for(int i = 0; i < m1; i++) for(int j = 0; j < n1; j++) *(x_new + (j)*m_new + (i)) = (*this)(i,j);
        
        delete[] x_;
        
        x_ = x_new;
        m_ = m_new;
        n_ = n_new;
        
        delete Q_;
        delete V_;
        delete R_;
        
        Q_= V_= 0;
        R_=0;
    }

    T& operator()(const int i,const int j)
    {assert(i>=0&&i<m_);assert(j>=0&&j<n_);return *(x_+j*m_+i);}

    const T& operator()(const int i,const int j) const
    {assert(i>=0&&i<m_);assert(j>=0&&j<n_);return *(x_+j*m_+i);}

    MATRIX_MXN<T>& operator=(const MATRIX_MXN<T>& A)
    {delete Q_;delete V_;delete R_;Q_=V_=0;R_=0;if(!x_ || m_*n_!=A.m*A.n){delete[] x_;x_=new T[A.m*A.n];};
    m_=A.m;n_=A.n;for(int k=0;k<m_*n_;k++) x_[k]=A.x[k];return *this;}

	MATRIX_MXN<T>& operator=(const T a)
    {int size=m_*n_;for(int i=0;i<size;i++)x_[i]=a;return *this;}

    MATRIX_MXN<T>& operator*=(const T a)
    {int size=m_*n_;for(int i=0;i<size;i++)x_[i]*=a;return *this;}

    MATRIX_MXN<T>& operator+=(const MATRIX_MXN<T>& A)
    {assert(n_==A.n&&m_==A.m);int size=m_*n_;for(int i=0;i<size;i++)x_[i]+=A.x[i];return *this;}

    MATRIX_MXN<T> operator-(const MATRIX_MXN<T>& A) const
    {assert(n_==A.n&&m_==A.m);int size=m_*n_;MATRIX_MXN<T> matrix(m_,n_);for(int i=0;i<size;i++) matrix.x[i]=x_[i]-A.x[i];return matrix;}

    MATRIX_MXN<T> operator-() const
    {int size=m_*n_;MATRIX_MX N<T> matrix(m_,n_);for(int i=0;i<size;i++) matrix.x[i]=-x_[i];return matrix;}

    VectorND<T> operator*(const VectorND<T>& y) const
    {
		assert(y.num_dimension_ == n_);

		VectorND<T> result(m_);
		for(int j=0;j<n_;j++) for(int i=0;i<m_;i++) 
			result[i]+=*(x_+(j)*m_+i)*y[j];return result;
	}

	void Transposed_Multiply(const VectorND<T>& y,VectorND<T>& result)//result=(*this).Transpose()*y
    {assert(y.n==m_ && result.n==n_);for(int i=0;i<m_;i++) for(int j=0;j<n_;j++) result(j)+=*(x_+(j)*m_+i)*y(i);}

	VectorND<T> Transposed_Multiply(const VectorND<T>& y) const //result=(*this).Transpose()*y
	{
		assert(y.num_dimension_==m_);
	
		VectorND<T> result;
		result.init(n_,true);

		for(int i=0;i<m_;i++) 
			for(int j=0;j<n_;j++) 
				result.values_[j]+=*(x_+j*m_+i)*y.values_[i];
			
		return result;
	}

	void Multiply(const VectorND<T>& y, VectorND<T>& result)
    {
        assert(y.num_dimension_ ==n_ && result.num_dimension_ == m_);
        
        for (int i = 0; i < m_; i++)
            result(i) = (T)0;

        for (int j = 0; j < n_; j++)
        {
            for (int i = 0; i < m_; i++)
            {
                result(i) += *(x_ + j*m_ + i) * y(j);

                /*               T a = *(x_ + j*m_ + i);
                               T b = y(j);
                               T c = result(i);

                               std::cout << "(" << a << " " << b << " " << c << ")" << std::flush;*/
            }
        }
        //std::cout << std::endl;
    }

    MATRIX_MXN<T> operator*(const MATRIX_MXN<T>& A) const
    {assert(n_==A.m);MATRIX_MXN<T> matrix(m_,A.n);for(int j=0;j<A.n;j++) for(int i=0;i<m_;i++) for(int k=0;k<n_;k++) matrix(i,j)+=*(x_+(k-1)*m_+i-1)*A(k,j);return matrix;}

    MATRIX_MXN<T> operator*(const MATRIX_NXN<T>& A) const
    {assert(n_==A.n_);MATRIX_MXN<T> matrix(m_,A.n_);for(int j=0;j<A.n_;j++) for(int i=0;i<m_;i++) for(int k=0;k<n_;k++) matrix(i,j)+=*(x_+(k-1)*m_+i-1)*A(k,j);return matrix;}

    MATRIX_MXN<T> Transpose()
    {MATRIX_MXN<T> matrix(n_,m_);for(int i=0;i<m_;i++) for(int j=0;j<n_;j++) matrix(j,i)=*(x_+(j-1)*m_+(i-1));return matrix;}

    VectorND<T> Transpose_Times(const VectorND<T>& y) const
    {assert(y.n==m_);VectorND<T> result(n_, true);for(int j=0;j<n_;j++) for(int i=0;i<m_;i++) result(j)+=*(x_+(j-1)*m_+i-1)*y(i);return result;}

    MATRIX_MXN<T> Transpose_Times(const MATRIX_MXN<T>& A) const
    {assert(n_==A.m);MATRIX_MXN<T> matrix(m_,A.n);for(int j=0;j<A.n;j++) for(int i=0;i<m_;i++) for(int k=0;k<n_;k++) matrix(i,j)+=*(x_+(i-1)*m_+k-1)*A(k,j);return matrix;}

    MATRIX_MXN<T> Permute_Columns(const VectorND<int>& p) const
    {assert(n_==p.n);MATRIX_MXN<T> x(m_,n_);for(int i=0;i<m_;i++)for(int j=0;j<=n_;j++) x(i,j)=(*this)(i,p(j));return x;}

    MATRIX_MXN<T> Unpermute_Columns(const VectorND<int>& p) const
    {assert(n_==p.n);MATRIX_MXN<T> x(m_,n_);for(int i=0;i<m_;i++)for(int j=0;j<=n_;j++) x(i,p(j))=(*this)(i,j);return x;}

    void Set_Zero_Matrix()
    {
        int size = m_ * n_;
        for(int i = 0; i < size; i++) x_[i] = 0;
    }

    //T Infinity_Norm() const
    //{T max_sum=0;
    //for(int j=0;j<n;j++){T sum=0;for(int i=0;i<m;i++) sum+=abs(*(x+(j-1)*m+i-1));max_sum=max(sum,max_sum);}
    //return max_sum;}

    T Frobenius_Norm_Squared() const
    {T sum=0;for(int k=0;k<m_*n_;k++) sum+=sqr(x_[k]);return sum;}

    //static MATRIX_MXN<T> Outer_Product(const VectorND<T> u,const VectorND<T> v)
    //{MATRIX_MXN<T> result(u.n,v.n);for(int i=1;i<=u.n;i++)for(int j=1;j<=v.n;j++)result(i,j)=u(i)*v(j);return result;}

    MATRIX_NXN<T> Normal_Equations_Matrix() const
    {
		MATRIX_NXN<T> result(n_);
		for(int j=0;j<n_;j++) for(int i=0;i<n_;i++) for(int k=0;k<m_;k++) 
			result(i,j) += *(x_+(i-1)*m_+k-1)*  *(x_+(j-1)*m_+k-1);
		return result;
	}

	MATRIX_NXN<T> Weighted_Normal_Equations_Matrix(const VectorND<T>& w) const//(W*(*this)).Normal_Equations_Matrix()
    {
		MATRIX_NXN<T> result(n_);
		for(int j=0;j<n_;j++) 
			for(int i=0;i<n_;i++) 
				for(int k=0;k<m_;k++) 
					result(i,j)+=*(x_+i*m_+k)* *(x_+j*m_+k)*w.values_[k]*w.values_[k];
					return result;
	}

    VectorND<T> Normal_Equations_Solve(const VectorND<T>& b) const
    {
        MATRIX_NXN<T> A_transpose_A(Normal_Equations_Matrix());
        VectorND<T> A_transpose_b(Transpose_Times(b));
        return A_transpose_A.Cholesky_Solve(A_transpose_b);
    }

    //void Gram_Schmidt_QR_Factorization()
    //{int i,j,k;if(Q) delete Q;Q=new MATRIX_MXN<T>(*this);if(R) delete(R);R=new MATRIX_NXN<T>(n);
    //for(j=1;j<=n;j++){ // for each column
    //    for(i=1;i<=m;i++) (*R)(j,j)+=sqr((*Q)(i,j));(*R)(j,j)=sqrt((*R)(j,j)); // compute the L2 norm
    //    T one_over_Rjj=1/(*R)(j,j);
    //    for(i=1;i<=m;i++) (*Q)(i,j)*=one_over_Rjj; // orthogonalize the column
    //    for(k=j+1;k<=n;k++){ // subtract this columns contributution from the rest of the columns
    //        for(i=1;i<=m;i++) (*R)(j,k)+=(*Q)(i,j)*(*Q)(i,k);
    //        for(i=1;i<=m;i++) (*Q)(i,k)-=(*R)(j,k)*(*Q)(i,j);}}}

    //VectorND<T> Gram_Schmidt_QR_Solve(const VectorND<T>& b)
    //{Gram_Schmidt_QR_Factorization();VectorND<T> c(Q->Transpose_Times(b));return R->Upper_Triangular_Solve(c);}

    //void Householder_QR_Factorization()
    //{int i,j,k;if(V) delete V;V=new MATRIX_MXN<T>(m,n);if(R) delete(R);R=new MATRIX_NXN<T>(n);MATRIX_MXN<T> temp(*this);
    //for(j=1;j<=n;j++){ // for each column
    //    VectorND<T> a(m);for(i=1;i<=m;i++) a(i)=temp(i,j);
    //    VectorND<T> v=a.Householder_Vector(j);for(i=1;i<=m;i++) (*V)(i,j)=v(i); // store the v's in V
    //    for(k=j;k<=n;k++){ // Householder transform each column
    //        for(i=1;i<=m;i++) a(i)=temp(i,k);
    //        VectorND<T> new_a=a.Householder_Transform(v);for(i=1;i<=m;i++) temp(i,k)=new_a(i);}}
    //for(i=1;i<=n;i++) for(j=1;j<=n;j++) (*R)(i,j)=temp(i,j);} // store R

    //static VectorND<T> Householder_Transform(const VectorND<T>& b,const MATRIX_MXN<T>& V)
    //{assert(V.m==b.n);VectorND<T> result(b);
    //for(int j=1;j<=V.n;j++){VectorND<T> v(V.m);for(int i=1;i<=V.m;i++) v(i)=V(i,j);result=result.Householder_Transform(v);}
    //return result;}

 //   VectorND<T> Householder_QR_Solve(const VectorND<T>& b)
 //   {Householder_QR_Factorization();VectorND<T> c=Householder_Transform(b,*V),c_short(n);for(int i=0;i<n;i++) c_short(i)=c(i);return R->Upper_Triangular_Solve(c_short);}

 //   static void Robust_Householder_QR_Solve(MATRIX_MXN<T>& A,VectorND<T>& b,VectorND<int>& p)
 //   {assert(A.m==b.n&&A.n==p.n);int m=A.m,n=A.n;VectorND<T> a(m);for(int i=0;i<a.n;i++) p(i)=i;
 //   for(int j=0;j<n;j++){int max_column=0;T max_column_norm=0;
 //       for(int k=j;k<=n;k++){T column_norm=0;for(int i=j;i<=m;i++) column_norm+=sqr(A(i,k));if(column_norm>max_column_norm){max_column_norm=column_norm;max_column=k;}}
 //       if(max_column_norm<FLT_MIN) return;if(max_column!=j){exchange(p(j),p(max_column));for(int i=0;i<m;i++) exchange(A(i,j),A(i,max_column));}
 //       A.Get_Column(j,a);VectorND<T> v=a.Householder_Vector(j);T two_over_v_dot_v=(T)2/v.Magnitude_Squared();
 //       if(A(j,j)>=0)A(j,j)=-sqrt(max_column_norm);else A(j,j)=sqrt(max_column_norm);for(int i=j+1;i<=m;i++)A(i,j)=(T)0;
 //       for(int k=j+1;k<=n;k++){T v_dot_a=0;for(int i=j;i<=m;i++) v_dot_a+=v(i)*A(i,k);T coefficient=v_dot_a*two_over_v_dot_v;for(int i=j;i<=m;i++) A(i,k)-=coefficient*v(i);}
 //       T v_dot_b=0;for(int i=j;i<=m;i++) v_dot_b+=v(i)*b(i);T coefficient=v_dot_b*two_over_v_dot_v;for(int i=j;i<=m;i++) b(i)-=coefficient*v(i);}}

    void Set_Column(const int j,const VectorND<T>& a)
    {assert(a.n==m_);for(int i=0;i<m_;i++) (*this)(i,j)=a(i);}

	void Set_Row(const int i,const VectorND<T>& a)
	{assert(a.num_dimension_==n_);for(int j=0;j<n_;j++) (*this)(i,j)=a.values_[j];}

    void Get_Column(const int j,VectorND<T>& a) const
    {assert(a.n==m_);for(int i=0;i<m_;i++) a(i)=(*this)(i,j);}
	
	VectorND<T> Get_Column(const int j) const
	{VectorND<T> a(m_);for(int i=0;i<m_;i++) a(i)=(*this)(i,j);return a;}

    //void Add_To_Submatrix(const int istart,const int jstart,const MATRIX_3X3<T>& a)
    //{for(int i=1;i<=3;i++) for(int j=1;j<=3;j++) (*this)(istart+i-1,jstart+j-1)+=a(i,j);}

 /*   void Add_To_Submatrix(const int istart,const int jstart,const SYMMETRIC_MATRIX_3X3<T>& a)
    {(*this)(istart,jstart)+=a.x11;(*this)(istart,jstart+1)+=a.x21;(*this)(istart+1,jstart)+=a.x21;(*this)(istart,jstart+2)+=a.x31;(*this)(istart+2,jstart)+=a.x31;
    (*this)(istart+1,jstart+1)+=a.x22;(*this)(istart+1,jstart+2)+=a.x32;(*this)(istart+2,jstart+1)+=a.x32;(*this)(istart+2,jstart+2)+=a.x33;}

    void Add_To_Submatrix(const int istart,const int jstart,const DIAGONAL_MATRIX_3X3<T>& a)
    {(*this)(istart,jstart)+=a.x11;(*this)(istart+1,jstart+1)+=a.x22;(*this)(istart+2,jstart+2)+=a.x33;}

  */
	void Add_To_Submatrix(const int istart,const int jstart,const VectorND<T>& a)
    {for(int i=0;i<a.n;i++) (*this)(istart+i-1,jstart)+=a(i);}

    void Add_To_Submatrix(const int istart,const int jstart,const MATRIX_MXN<T>& a)
    {for(int i=0;i<a.m;i++) for(int j=0;j<a.n;j++) (*this)(istart+i-1,jstart+j-1)+=a(i,j);}

    void Add_To_Submatrix(const int istart,const int jstart,const MATRIX_NXN<T>& a)
    {for(int i=0;i<a.n_;i++) for(int j=0;j<a.n_;j++) (*this)(istart+i-1,jstart+j-1)+=a(i,j);}

    //void Get_Submatrix(const int istart,const int jstart,MATRIX_MXN<T>& a) const
    //{for(int i=0;i<a.m;i++) for(int j=0;j<a.n;j++) a(i,j)=(*this)(istart+i-1,jstart+j-1);}

    //void Get_Submatrix(const int istart,const int jstart,MATRIX_NXN<T>& a) const
    //{for(int i=0;i<a.n;i++) for(int j=0;j<a.n;j++) a(i,j)=(*this)(istart+i-1,jstart+j-1);}

    //void Set_Submatrix(const int istart,const int jstart,MATRIX_MXN<T>& a)
    //{for(int i=0;i<a.m;i++) for(int j=0;j<a.n;j++) (*this)(istart+i-1,jstart+j-1)=a(i,j);}

    //void Set_Submatrix(const int istart,const int jstart,MATRIX_NXN<T>& a)
    //{for(int i=0;i<a.n;i++) for(int j=0;j<a.n;j++) (*this)(istart+i-1,jstart+j-1)=a(i,j);}

//#####################################################################
};
template<class T> inline std::ostream& operator<<(std::ostream& output_stream,const MATRIX_MXN<T>& A)
{for(int i=0;i<A.m_;i++){for(int j=0;j<A.n_;j++)output_stream<<A(i,j)<<" ";output_stream<<std::endl;}return output_stream;}
//{for(int i=0;i<a.m;i++){for(int j=0;j<a.n;j++){output_stream.flags(std::ios::right);output_stream.width(12);output_stream<<A(i,j);}output_stream<<std::endl;}return output_stream;}

template<class T> inline MATRIX_MXN<T> operator*(const MATRIX_NXN<T>& A,const MATRIX_MXN<T>& B)
{assert(A.n_==B.m);MATRIX_MXN<T> matrix(B.m,B.n);for(int j=0;j<B.n;j++) for(int i=0;i<a.n;i++) for(int k=0;k<B.m;k++) matrix(i,j)+=A(i,k)*B(k,j);return matrix;}

