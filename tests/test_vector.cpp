#include <iostream>
#include <string>
#include <utility>
#include <gtest/gtest.h>
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

TEST( VectorTest, PushAndAccess )
{
    ben::vector<int> v;
    EXPECT_EQ( v.size(), 0 );
    EXPECT_EQ( v.capacity(), 0 );

    v.push_back( 10 );
    EXPECT_EQ( v.size(), 1 );
    EXPECT_EQ( v[ 0 ], 10 );

    v.push_back( 20 );
    EXPECT_EQ( v.size(), 2 );
    EXPECT_EQ( v[ 1 ], 20 );

    for ( int i = 0; i < 100; ++i )
    {
        v.push_back( i );
    }
    EXPECT_EQ( v.size(), 102 );
    EXPECT_EQ( v[ 101 ], 99 );
}

TEST( VectorTest, Strings )
{
    ben::vector<std::string> v;
    std::string s = "l-value";
    
    v.push_back( s );
    v.push_back( "r-value" );

    EXPECT_EQ( v[ 0 ], "l-value" );
    EXPECT_EQ( v[ 1 ], "r-value" );
    EXPECT_EQ( v.size(), 2 );
}

TEST( VectorTest, CopySemantics )
{
    ben::vector<int> v1;
    v1.push_back( 1 );
    v1.push_back( 2 );

    ben::vector<int> v2( v1 );
    
    EXPECT_EQ( v2.size(), 2 );
    EXPECT_EQ( v2[ 0 ], 1 );
    EXPECT_EQ( v2[ 1 ], 2 );

    v2[ 0 ] = 999;
    EXPECT_EQ( v1[ 0 ], 1 );
    EXPECT_EQ( v2[ 0 ], 999 );

    ben::vector<int> v3;
    v3 = v1; 
    EXPECT_EQ( v3.size(), 2 );
    EXPECT_EQ( v3[ 0 ], 1 );
}

TEST( VectorTest, MoveSemantics )
{
    ben::vector<std::string> v1;
    v1.push_back( "move_me" );

    ben::vector<std::string> v2( std::move( v1 ) );

    EXPECT_EQ( v2.size(), 1 );
    EXPECT_EQ( v2[ 0 ], "move_me" );
    EXPECT_EQ( v1.size(), 0 );
    EXPECT_EQ( v1.capacity(), 0 );

    ben::vector<std::string> v3;
    v3 = std::move( v2 );

    EXPECT_EQ( v3.size(), 1 );
    EXPECT_EQ( v3[ 0 ], "move_me" );
    EXPECT_EQ( v2.size(), 0 );
}

TEST( VectorTest, Reserve )
{
    ben::vector<int> v;
    v.reserve( 100 );

    EXPECT_EQ( v.capacity(), 100 );
    EXPECT_EQ( v.size(), 0 );

    v.push_back( 1 );
    EXPECT_EQ( v.capacity(), 100 );
}

TEST( VectorTest, MemoryLeaksAndDestruction )
{
    EXPECT_EQ( Resource::count, 0 );

    {
        ben::vector<Resource> v;
        v.push_back( Resource( 10 ) );
        v.push_back( Resource( 20 ) );
        EXPECT_EQ( Resource::count, 2 );
        
        v.reserve( 10 );
        EXPECT_EQ( Resource::count, 2 );
    }

    EXPECT_EQ( Resource::count, 0 );
}

TEST( VectorTest, EmplaceBack )
{
    ben::vector<Resource> v;
    v.emplace_back( 50 );
    EXPECT_EQ( v.size(), 1 );
    EXPECT_EQ( v[ 0 ].value, 50 );
}

int main( int argc, char **argv )
{
    ::testing::InitGoogleTest( &argc, argv );
    return RUN_ALL_TESTS();
}
