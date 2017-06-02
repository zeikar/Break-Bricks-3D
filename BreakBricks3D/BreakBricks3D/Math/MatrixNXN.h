/*
Authored by Prof. Jeong-Mo Hong, CSE Dongguk University
for Introduction to Computer Graphics, 2017 Spring
*/

#pragma once

#include "DataStructure/VectorND.h"
#include "MatrixMXN.h"

#ifndef __exchange__
#define __exchange__
template<class T> inline void exchange(T& a,T& b) {T c=a;a=b;b=c;}
#endif

template<class T> class MATRIX_MXN;

template<class T>
class MATRIX_NXN
{
public:
    int n_; // size of the n by n matrix
    T *x; // pointer to the one dimensional data
    T small_number_;
    MATRIX_NXN<T> *L,*U_,*inverse_;
    VectorND<int> *p_;

    MATRIX_NXN()
        :n_(0),x(0),small_number_((T)1e-8),L(0),U_(0),inverse_(0),p_(0)
    {}

    MATRIX_NXN(const int n_input)
        :n_(n_input),small_number_((T)1e-8),L(0),U_(0),inverse_(0),p_(0)
    {
        x=new T[n_*n_];
        for(int k=0;k<n_*n_;k++) x[k]=0;
    }

    MATRIX_NXN(const MATRIX_NXN<T>& matrix_input)
        :n_(matrix_input.n_),small_number_(matrix_input.small_number_),L(0),U_(0),inverse_(0),p_(0)
    {
        x=new T[n_*n_];
        for(int k=0;k<n_*n_;k++) x[k]=matrix_input.x[k];
    }

    template <class T2> explicit MATRIX_NXN(const MATRIX_NXN<T2>& matrix_input)
        :n_(matrix_input.n_),small_number_(matrix_input.small_number_),L(0),U_(0),inverse_(0),p_(0)
    {
        x=new T[n_*n_];
        for(int k=0;k<n_*n_;k++) x[k]=(T)matrix_input.x[k];
    }

    ~MATRIX_NXN()
    {
		if(x) delete [] x;
		if(L) delete L;
		if(U_) delete(U_);
		if(p_) delete p_;
		if(inverse_) delete inverse_;
	}

	void Resize(const int n_input)
	{n_=n_input;
	if(x)delete [] x;
	x=new T[n_*n_];
	for(int k=0;k<n_*n_;k++) x[k]=0;}

    T& operator()(const int i,const int j)
    {assert(i >= 0 && i < n_);assert(j >= 0 && j <= n_);return *(x+i*n_+j);}

    const T& operator()(const int i,const int j) const
    {assert(i >= 0 && i < n_);assert(j >= 0 && j <= n_);return *(x+i*n_+j);}

    bool operator==(const MATRIX_NXN<T>& A) const
    {assert(A.n_==n_);for(int k=0;k<n_*n_;k++) if(x[k]!=A.x[k]) return false;return true;}

    bool operator!=(const MATRIX_NXN<T>& A) const
    {assert(A.n_==n_);for(int k=0;k<n_*n_;k++) if(x[k]!=A.x[k]) return true;return false;}

    MATRIX_NXN<T>& operator=(const MATRIX_NXN<T>& A)
    {delete L;delete U_;delete inverse_;delete p_;L=U_=inverse_=0;p_=0;if(!x || n_!=A.n_){delete[] x;x=new T[A.n_*A.n_];}
    n_=A.n_;for(int k=0;k<n_*n_;k++) x[k]=A.x[k];return *this;}

    MATRIX_NXN<T>& operator+=(const MATRIX_NXN<T>& A)
    {for(int i=0;i<n_*n_;i++) x[i]+=A.x[i];return *this;}

    MATRIX_NXN<T>& operator-=(const MATRIX_NXN<T>& A)
    {for(int i=0;i<n_*n_;i++) x[i]-=A.x[i];return *this;}

	MATRIX_NXN<T>& operator=(const T a)
    {for(int i=0;i<n_;i++) for(int j=0;j<n_;j++) (*(x+i*n_+j))=a;return *this;}

    MATRIX_NXN<T>& operator*=(const T a)
    {for(int i=0;i<n_;i++) for(int j=0;j<n_;j++) (*(x+i*n_+j))*=a;return *this;}

    MATRIX_NXN<T> operator+(const MATRIX_NXN<T>& A) const
    {assert(A.n_==n_);MATRIX_NXN<T> result(n_, true);for(int i=0;i<n_*n_;i++) result.x[i]=x[i]+A.x[i];return result;}

    MATRIX_NXN<T> operator-(const MATRIX_NXN<T>& A) const
    {assert(A.n_==n_);MATRIX_NXN<T> result(n_,true);for(int i=0;i<n_*n_;i++) result.x[i]=x[i]-A.x[i];return result;}

    MATRIX_NXN<T> operator*(const T a) const
    {MATRIX_NXN<T> result(n_,true);for(int i=0;i<n_;i++) for(int j=0;j<n_;j++) (*(result.x+i*n_+j))=(*(x+i*n_+j))*a;return result;}

    VectorND<T> operator*(const VectorND<T>& y) const
    {VectorND<T> result(n_,true);Multiply(y,result);return result;}

    void Multiply(const VectorND<T>& y,VectorND<T>& result) const
    {assert(y.n==n_ && result.n==n_);for(int i=0;i<n_;i++) for(int j=0;j<n_;j++) (*(result.x+i))+=(*(x+i*n_+j))*(*(y.x+j));}

    VectorND<T> Transpose_Times(const VectorND<T>& y) const
    {assert(y.n==n_);VectorND<T> result(n_);for(int j=0;j<n_;j++) for(int i=0;i<n_;i++) (*(result.x+i))+=(*(x+j*n_+i))*(*(y.x+j));return result;}

    MATRIX_NXN<T> Transpose_Times(const MATRIX_NXN<T>& A) const
    {assert(n_==A.n_);MATRIX_NXN<T> matrix(n_);for(int j=0;j<a.n;j++) for(int i=0;i<n_;i++) for(int k=1;k<=n_;k++) matrix(i,j)+=(*this)(k,i)*A(k,j);return matrix;}

    MATRIX_NXN<T> operator*(const MATRIX_NXN<T>& A) const
    {assert(A.n_==n_);MATRIX_NXN<T> result(n_);for(int i=0;i<n_;i++){for(int j=0;j<n_;j++){T total=(T)0;for(int k=0;k<n_;k++) total+=(*(x+i*n_+k))*(*(A.x+k*n_+j));
    (*(result.x+i*n_+j))=total;}}return result;}

    MATRIX_NXN<T> Permute_Columns(const VectorND<int>& p) const
    {assert(n_==p.n);MATRIX_NXN<T> x(n_);for(int i=0;i<n_;i++)for(int j=0;j<n_;j++) x(i,j)=(*this)(i,p(j));return x;}

    MATRIX_NXN<T> Unpermute_Columns(const VectorND<int>& p) const
    {assert(n_==p.n);MATRIX_NXN<T> x(n_);for(int i=0;i<n_;i++)for(int j=0;j<n_;j++) x(i,p(j))=(*this)(i,j);return x;}

    //void Set_Identity_Matrix()
    //{Set_Zero_Matrix();for(int i=0;i<n;i++) (*this)(i,i)=1;}

    //void Set_Zero_Matrix()
    //{for(int i=0;i<n*n;i++) x[i]=0;}

    //static MATRIX_NXN<T> Identity_Matrix(const int n)
    //{MATRIX_NXN<T> A(n);for(int i=0;i<n;i++) A(i,i)=(T)1;return A;}

    //MATRIX_NXN<T> Transpose() const
    //{MATRIX_NXN<T> A(n);for(int i=0;i<n;i++) for(int j=0;j<n;j++) A(j,i)=*(x+(i-1)*n+(j-1));return A;}

    //T Trace() const
    //{T trace=0;for(int i=0;i<n;i++) trace+=(*(x+(i-1)*n+(i-1)));return trace;}

    //static MATRIX_NXN<T> Outer_Product(const VectorND<T>& u,const VectorND<T>& v)
    //{MATRIX_NXN<T> result(u.n);assert(u.n==v.n);for(int i=1;i<=u.n;i++)for(int j=1;j<=u.n;j++)result(i,j)=u(i)*v(j);return result;}

    //MATRIX_NXN<T> Normal_Equations_Matrix() const
    //{MATRIX_NXN<T> result(n,true);for(int k=1;k<=n;k++) for(int i=0;i<n;i++) for(int j=0;j<n;j++) result(i,j)+=*(x+(k-1)*n+i-1)**(x+(k-1)*n+j-1);return result;}

    VectorND<T> Lower_Triangular_Solve(const VectorND<T>& b) const
    {
		assert(n_==b.num_dimension_);
		VectorND<T> x(n_);
		for(int i=0;i<n_;i++)
		{
			x.values_[i]=b.values_[i];

			for(int j=0;j<=i-1;j++) 
				x.values_[i]-=(*this)(i,j)*x.values_[j];
			
			x.values_[i]/=(*this)(i,i);
		}
		return x;
	}

    VectorND<T> Transpose_Lower_Triangular_Solve(const VectorND<T>& b) const
    {assert(n_==b.n);VectorND<T> x(b);
    for(int i=0;i<n_;i++){x.values_[i]/=(*this)(i,i);for(int j=i+1;j<n_;j++) x.values_[j]-=(*this)(i,j)*x.values_[i];}
    return x;}

    VectorND<T> Upper_Triangular_Solve(const VectorND<T>& b) const
    {
		assert(n_==b.num_dimension_);
		VectorND<T> x(n_);
		for(int i=n_-1;i>=0;i--)
		{
			x.values_[i]=b[i];
			for(int j=n_-1;j>=i+1;j--) 
				x.values_[i]-=(*this)(i,j)*x.values_[j];

			x.values_[i]/=(*this)(i,i);
		}
		return x;
	}

    VectorND<T> Transpose_Upper_Triangular_Solve(const VectorND<T>& b) const
    {assert(n_==b.n);VectorND<T> x(b);
    for(int i=n_;i>=1;i--){x.values_[i]/=(*this)(i,i);for(int j=i-1;j>=1;j--) x.values_[j]-=(*this)(i,j)*x.values_[i];}
    return x;}

    //MATRIX_MXN<T> Upper_Triangular_Solve(const MATRIX_MXN<T>& b) const
    //{assert(n==b.m);MATRIX_MXN<T> x(b.m,b.n);
    //for(int bcol=1;bcol<=b.n;bcol++)for(int i=n;i>=1;i--){x(i,bcol)=b(i,bcol);for(int j=n;j>=i+1;j--) x(i,bcol)-=(*this)(i,j)*x(j,bcol);x(i,bcol)/=(*this)(i,i);}
    //return x;}

    void LU_Factorization()
    {int i,j,k;if(L) delete L;L=new MATRIX_NXN<T>(n_);if(U_) delete(U_);U_=new MATRIX_NXN<T>(*this);
    for(j=0;j<n_;j++){ // for each column
        for(i=j;i<n_;i++) (*L)(i,j)=(*U_)(i,j)/(*U_)(j,j); // fill in the column for L
        for(i=j+1;i<n_;i++) for(k=j;k<n_;k++) (*U_)(i,k)-=(*L)(i,j)*(*U_)(j,k);}} // sweep across each row below row j

    VectorND<T> LU_Solve(const VectorND<T>& b)\
    {LU_Factorization();return U_->Upper_Triangular_Solve(L->Lower_Triangular_Solve(b));}

    void LU_Inverse()
    {if(inverse_) delete inverse_;inverse_=new MATRIX_NXN<T>(n_);
    LU_Factorization();
    for(int j=0;j<n_;j++){ // for each column
        VectorND<T> b(n_);b(j)=1; // piece of the identity matrix
        VectorND<T> x=U_->Upper_Triangular_Solve(L->Lower_Triangular_Solve(b));
        for(int i=0;i<n_;i++) (*inverse_)(i,j)=x.values_[i];}}

    void PLU_Factorization()
    {int i,j,k;if(L) delete L;L=new MATRIX_NXN<T>(n_);if(U_) delete(U_);U_=new MATRIX_NXN<T>(*this);
    if(p_) delete p_;p_=new VectorND<int>(n_,false);for(i=0;i<n_;i++)(*p_)(i)=i; // initialize p
    for(j=0;j<n_;j++){ // for each column
        // find the largest element and switch rows
        int row=j;T value=abs((*U_)(j,j));
        for(i=j+1;i<n_;i++) if(abs((*U_)(i,j)) > value){row=i;value=abs((*U_)(i,j));}
        if(row != j){ // need to switch rows
            exchange((*p_)(j),(*p_)(row)); // update permutation matrix
            for(k=0;k<j-1;k++) exchange((*L)(j,k),(*L)(row,k)); // update L
            for(k=j;k<n_;k++) exchange((*U_)(j,k),(*U_)(row,k));} // update U
        // standard LU factorization steps
        for(i=j;i<n_;i++) (*L)(i,j)=(*U_)(i,j)/(*U_)(j,j); // fill in the column for L
        for(i=j+1;i<n_;i++) for(k=j;k<n_;k++) (*U_)(i,k)-=(*L)(i,j)*(*U_)(j,k);}} // sweep across each row below row j

    VectorND<T> PLU_Solve(const VectorND<T>& b)
    {
		PLU_Factorization();
		VectorND<T> x=b;
		return U_->Upper_Triangular_Solve(L->Lower_Triangular_Solve(x.Permute(*p_)));
	}

    //void PLU_Inverse()
    //{if(inverse) delete inverse;inverse=new MATRIX_NXN<T>(n);
    //PLU_Factorization();
    //for(int j=0;j<n;j++){ // for each column
    //    VectorND<T> b(n);b(j)=1; // piece of the identity matrix
    //    VectorND<T> x=U->Upper_Triangular_Solve(L->Lower_Triangular_Solve(b.Permute(*p)));
    //    for(int i=0;i<n;i++) (*inverse)(i,j)=x.val[i];}}

    //void Gauss_Seidel_Single_Iteration(VectorND<T>& x,const VectorND<T>& b) const
    //{assert(x.n==b.n && x.n==n);
    //for(int i=1;i<= n;i++){
    //    T rho=0;
    //    for(int j=1;j<i;j++) rho+=(*this)(i,j)*x.val[j];
    //    for(int j=i+1;j<=n;j++) rho+=(*this)(i,j)*x.val[j];
    //    x.val[i]=(b[i]-rho)/(*this)(i,i);}}

    //T Norm() const // L_infinity norm - maximum row sum
    //{T max_sum=0;
    //for(int i=0;i<n;i++){T sum=0;for(int j=0;j<n;j++) sum+=abs((*this)(i,j));max_sum=max(sum,max_sum);}
    //return max_sum;}

    //T Condition_Number()
    //{PLU_Inverse();return Norm()*inverse->Norm();} // makes sure the inverse is up to date

    void Cholesky_Factorization() // LL^{transpose} factorization
    {
		int i,j,k;

		if(L) delete L;
		L=new MATRIX_NXN<T>(n_);

		if(U_) delete(U_);
		U_=new MATRIX_NXN<T>(*this);
		
		for(j=0;j<n_;j++)
		{ // for each column
			for(k=0;k<=j-1;k++) 
				for(i=j;i<n_;i++)
					(*U_)(i,j)-=(*L)(j,k)*(*L)(i,k); // subtract off the known stuff in previous columns
			
			(*L)(j,j)=sqrt((*U_)(j,j));
		
			for(i=j+1;i<n_;i++) 
				(*L)(i,j)=(*U_)(i,j)/(*L)(j,j);
		} // update L
		for(i=0;i<n_;i++) 
			for(j=0;j<n_;j++) 
				(*U_)(i,j)=(*L)(j,i);

	} // put L^{transpose} into U

    VectorND<T> Cholesky_Solve(const VectorND<T>& b)
    {
		Cholesky_Factorization();
		return U_->Upper_Triangular_Solve(L->Lower_Triangular_Solve(b));
	}

    void Cholesky_Inverse()
    {if(inverse_) delete inverse_;inverse_=new MATRIX_NXN<T>(n_);
    Cholesky_Factorization();
    for(int j=0;j<n_;j++){ // for each column
        VectorND<T> b(n_);b(j)=1; // piece of the identity matrix
        VectorND<T> x=U_->Upper_Triangular_Solve(L->Lower_Triangular_Solve(b));
        for(int i=0;i<n_;i++) (*inverse_)(i,j)=x.values_[i];}}

    void Set_Column(const int j,const VectorND<T>& a)
    {assert(a.n==n_);for(int i=0;i<n_;i++) (*this)(i,j)=a(i);}

   void Get_Column(const int j,VectorND<T>& a) const
    {assert(a.n==n_);for(int i=0;i<n_;i++) a(i)=(*this)(i,j);}

	VectorND<T> Get_Column(const int j) const
	{VectorND<T> a(n_);for(int i=0;i<n_;i++) a(i)=(*this)(i,j);return a;}

   /*  void Add_To_Submatrix(const int istart,const int jstart,const MATRIX_3X3<T>& a)
    {for(int i=1;i<=3;i++) for(int j=1;j<=3;j++) (*this)(istart+i-1,jstart+j-1)+=a(i,j);}*/

    //void Add_To_Submatrix(const int istart,const int jstart,const SYMMETRIC_MATRIX_3X3<T>& a)
    //{(*this)(istart,jstart)+=a.x11;(*this)(istart,jstart+1)+=a.x21;(*this)(istart+1,jstart)+=a.x21;(*this)(istart,jstart+2)+=a.x31;(*this)(istart+2,jstart)+=a.x31;
    //(*this)(istart+1,jstart+1)+=a.x22;(*this)(istart+1,jstart+2)+=a.x32;(*this)(istart+2,jstart+1)+=a.x32;(*this)(istart+2,jstart+2)+=a.x33;}

    void Add_To_Submatrix(const int istart,const int jstart,const MATRIX_MXN<T>& a)
    {for(int i=0;i<a.m;i++) for(int j=0;j<a.n;j++) (*this)(istart+i,jstart+j)+=a(i,j);}

//#####################################################################
};

template<class T> inline MATRIX_NXN<T> operator*(const T a,const MATRIX_NXN<T>& A)
{MATRIX_NXN<T> result(A.n_);for(int i=0;i<A.n_;i++) for(int j=0;j<A.n_;j++) (*(result.x+i*A.n_+j))=a*(*(A.x+i*A.n_+j));return result;}

template<class T> inline MATRIX_NXN<T> operator*(const int a,const MATRIX_NXN<T>& A)
{MATRIX_NXN<T> result(A.n_);for(int i=0;i<A.n_;i++) for(int j=0;j<A.n_;j++) (*(result.x+i*A.n_+j))=a*(*(A.x+i*A.n_+j));return result;}

template<class T> inline std::istream& operator>>(std::istream& input_stream,MATRIX_NXN<T>& A)
{for(int i=0;i<A.n_;i++)for(int j=0;j<a.n;j++)input_stream>>A(i,j);return input_stream;}

template<class T> inline std::ostream& operator<<(std::ostream& output_stream,const MATRIX_NXN<T>& A)
{for(int i=0;i<A.n_;i++){for(int j=0;j<A.n_;j++)output_stream<<A(i,j)<<" ";output_stream<<std::endl;}return output_stream;}
//{for(int i=0;i<a.m;i++){for(int j=0;j<a.n;j++){output_stream.flags(std::ios::right);output_stream.width(12);output_stream<<A(i,j);}output_stream<<std::endl;}return output_stream;}




