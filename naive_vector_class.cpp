#include <iostream>

// template <class T>
template <typename T>
class vector{
private:
    T* elements_;
    unsigned capacity_;
    unsigned size_;

    void reserve(unsigned newalloc){
        if (capacity_ >= newalloc) return;
        
        try{
            T* new_elements_ = new T[newalloc]();
            capacity_ = newalloc;
            for (int i = 0; i < size_; i++) {
                new_elements_[i] = elements_[i];
            }
            delete[] elements_;
            elements_ = new_elements_;
            
        } catch (const std::bad_alloc& e) {
            std::cout<< e.what() << std::endl;
        }
        
    }
    
public:
    explicit vector(): size_(0), capacity_(10){
        elements_ = new T[capacity_];
    };
    vector(std::initializer_list<T> );
    
    ~vector() {
        delete[] elements_;
    }
    //copy constructor
    vector(const vector& other);
    // copy assignment
    vector& operator=(const vector& other);
    // move constructor
    vector(vector&& other) noexcept;
    // move assignment
    vector& operator=(vector&& other);
    
    explicit vector(unsigned size, const T& val = T()) {
        try {
            elements_ = new T[2 * size]; // zero initialized
            capacity_ = 2 * size;
            size_ = size;
            for (int i = 0; i < size_; i++) {
                elements_[i] = val;
            }
            
        } catch (const std::bad_alloc& e) {
            std::cout << e.what() << '\n';
        }
        
        // https://en.cppreference.com/w/cpp/memory/new/nothrow
        //        elements_ = new (std::nothrow) int[2*size]();
        //        if (!elements) {
        //            std::cout << "Memory allocation failed!\n";
        //        }
        
    }
    
    void push_back(const T& num) {
        if (size_ == 0) {
            reserve(2);
        }
        else if (size_ >= capacity_) {
            reserve(2 * capacity_);
        }
        elements_[size_] = num;
        size_++;
    }
    
    
    void insert(unsigned index, const T& val) {
        if (index < 0 || index > size_) {
            std::cout << "invalid index" << std::endl;
        }
        push_back(val); //increase size by 1
        for (int i = size_ - 1; i > index; i--) {
            elements_[i] = elements_[i - 1];
        }
        elements_[index] = val;
    }
    
    void pop_back() {
        size_--;
    }
    
    // operator[] must return a reference in order
    // to work properly with assignment
    // v[5] = 4;
    T& operator[] (unsigned index) {
        return elements_[index];
    }
    
    // for const vector
    const T& operator[] (unsigned index) const {
        return elements_[index];
    }
    
    void print() const;
    
};

template<typename T>
void vector<T>::print() const {
    for (int i = 0; i < size_; i++) {
        std::cout << elements_[i] << " ";
    }
    std::cout<< std::endl;
}

template<typename T>
vector<T>::vector(const vector& other): elements_(new T[other.capacity_]),
                                        size_(other.size_),
                                        capacity_(other.capacity_){
    std::cout << "copy constructor" << std::endl;
    std::copy(other.elements_, other.elements_ + other.size_, elements_);
}

template<typename T>
vector<T>& vector<T>::operator=(const vector& other){
//    if (this != &other) {
//        delete[] elements_; // free existing resource
//        elements_ = new T[other.capacity_];
//        std::copy(other.elements_, other.elements_ + other.size_, elements_);
//        size_ = other.size_;
//        capacity_ = other.capacity_;
//    }
//    return *this;
    
    std::cout << "copy assignment" << std::endl;
    if (this == &other) return *this;
    vector temp{other};
    std::swap(*this, temp); // object need to be move constructable and assignable
    return *this;
    // old elements are destroyed by temp's destructor
}

template<typename T>
vector<T>::vector(vector&& other) noexcept{
    std::cout << "move constructor" << std::endl;
    // copy data pointer and its length from source object.
    elements_ = other.elements_;
    size_ = other.size_;
    capacity_ = other.capacity_;
    // Release the data pointer from the source object so that
    // the destructor does not freee the memory multiple times.
    other.elements_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
}

template<typename T>
vector<T>& vector<T>::operator=(vector&& other) {
    std::cout << "move assignment" << std::endl;
    if (this == &other) return *this;
    
    delete[] elements_; // free existing resource
    elements_ = other.elements_;
    size_ = other.size_;
    capacity_ = other.capacity_;
    
    other.elements_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
    return *this;
}

template <typename T>
vector<T>::vector(std::initializer_list<T> elements) {
    for(const auto &element : elements) {
        push_back(element);
    }
}

class A {
    int mem;
public:
    A(){std::cout << "default constructor " <<mem << std::endl;}
    A(int k) : mem(k){ std::cout << "defined constructor " <<mem << std::endl;}
};

int main() {
    vector<double> a(1, 3.0);
    a.push_back(3);
    a.push_back(4);
    a.push_back(5);
    a.insert(4, 30);
    a.insert(2, 10);
    a.insert(0, 20);
    a.print();
    
    const vector<double> b(3);
    b.print();
    
    vector<double> c{a};
    c.print();
    c = b;
    c.print();
    
    vector<double> d(std::move(c));
    d.print();
    
    c = vector<double>(3, 6.3);
    c.print();
    
    vector<double> e = {3.5, 4.2, 5.5, 5.5};
    e.print();
    
    vector<A> k(2, A(3));
    
//    vector c = vector();
//    c.push_back(8);
//    c.push_back(9);
//    c.print();
    
    return 0;
}
