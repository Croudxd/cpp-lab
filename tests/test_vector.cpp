#include <iostream>
#include <cassert>
#include <string>
#include <utility>
#include "ben/vector.hpp"

struct Resource
{
    static int count;
    int value;

    Resource() : value( 0 )
    {
        count++;
    }

    Resource( int v ) : value( v )
    {
        count++;
    }

    Resource( const Resource& other ) : value( other.value )
    {
        count++;
    }

    Resource( Resource&& other ) noexcept : value( other.value )
    {
        other.value = -1;
        count++;
    }

    ~Resource()
    {
        count--;
    }

    Resource& operator=( const Resource& other )
    {
        value = other.value;
        return *this;
    }

    Resource& operator=( Resource&& other ) noexcept
    {
        value = other.value;
        other.value = -1;
        return *this;
    }
};

int Resource::count = 0;

void test_push_and_access()
{
    ben::vector<int> v;
    assert( v.size() == 0 );
    assert( v.capacity() == 0 );

    v.push_back( 10 );
    assert( v.size() == 1 );
    assert( v[ 0 ] == 10 );

    v.push_back( 20 );
    assert( v.size() == 2 );
    assert( v[ 1 ] == 20 );

    for ( int i = 0; i < 100; ++i )
    {
        v.push_back( i );
    }
    assert( v.size() == 102 );
    assert( v[ 101 ] == 99 );
}

void test_strings()
{
    ben::vector<std::string> v;
    std::string s = "l-value";
    
    v.push_back( s );
    v.push_back( "r-value" );

    assert( v[ 0 ] == "l-value" );
    assert( v[ 1 ] == "r-value" );
    assert( v.size() == 2 );
}

void test_copy_semantics()
{
    ben::vector<int> v1;
    v1.push_back( 1 );
    v1.push_back( 2 );

    ben::vector<int> v2( v1 );
    
    assert( v2.size() == 2 );
    assert( v2[ 0 ] == 1 );
    assert( v2[ 1 ] == 2 );

    v2[ 0 ] = 999;
    assert( v1[ 0 ] == 1 );
    assert( v2[ 0 ] == 999 );

    ben::vector<int> v3;
    v3 = v1; 
    assert( v3.size() == 2 );
    assert( v3[ 0 ] == 1 );
}

void test_move_semantics()
{
    ben::vector<std::string> v1;
    v1.push_back( "move_me" );

    ben::vector<std::string> v2( std::move( v1 ) );

    assert( v2.size() == 1 );
    assert( v2[ 0 ] == "move_me" );
    assert( v1.size() == 0 );
    assert( v1.capacity() == 0 );

    ben::vector<std::string> v3;
    v3 = std::move( v2 );

    assert( v3.size() == 1 );
    assert( v3[ 0 ] == "move_me" );
    assert( v2.size() == 0 );
}

void test_reserve()
{
    ben::vector<int> v;
    v.reserve( 100 );

    assert( v.capacity() == 100 );
    assert( v.size() == 0 );

    v.push_back( 1 );
    assert( v.capacity() == 100 );
}

void test_memory_leaks_and_destruction()
{
    assert( Resource::count == 0 );

    {
        ben::vector<Resource> v;
        v.push_back( Resource( 10 ) );
        v.push_back( Resource( 20 ) );
        assert( Resource::count == 2 );
        
        v.reserve( 10 );
        assert( Resource::count == 2 );
    }

    assert( Resource::count == 0 );
}

void test_emplace_back()
{
    ben::vector<Resource> v;
    v.emplace_back( 50 );
    assert( v.size() == 1 );
    assert( v[ 0 ].value == 50 );
}

int main()
{
    test_push_and_access();
    test_strings();
    test_copy_semantics();
    test_move_semantics();
    test_reserve();
    test_memory_leaks_and_destruction();
    test_emplace_back();

    std::cout << "All tests passed successfully." << std::endl;
    return 0;
}
