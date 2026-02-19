#include <iostream>
#include <gtest/gtest.h>

#include "ben/shared_ptr.hpp"

struct Tracker
{
    static int alive_count;
    int id;

    Tracker ( int val ) : id( val )
    {
        alive_count++;
    }

    ~Tracker ()
    {
        alive_count--;
    }
};

int Tracker::alive_count = 0;

TEST( SharedPtrTest, ConstructorAndDestructor )
{
    EXPECT_EQ( Tracker::alive_count, 0 );
    {
        ben::shared_ptr<Tracker> sp( new Tracker( 10 ) );
        EXPECT_EQ( Tracker::alive_count, 1 );
        EXPECT_EQ( sp->id, 10 );
        EXPECT_EQ( sp.use_count(), 1 );
    }
    EXPECT_EQ( Tracker::alive_count, 0 );
}

TEST( SharedPtrTest, CopyLogic )
{
    EXPECT_EQ( Tracker::alive_count, 0 );
    {
        ben::shared_ptr<Tracker> sp1( new Tracker( 20 ) );
        {
            ben::shared_ptr<Tracker> sp2 = sp1;
            
            EXPECT_EQ( Tracker::alive_count, 1 );
            EXPECT_EQ( sp1.use_count(), 2 );
            EXPECT_EQ( sp2.use_count(), 2 );
            EXPECT_EQ( sp2->id, 20 );
        } 
        EXPECT_EQ( Tracker::alive_count, 1 );
        EXPECT_EQ( sp1.use_count(), 1 );
    }
    EXPECT_EQ( Tracker::alive_count, 0 );
}

TEST( SharedPtrTest, AssignmentOperator )
{
    EXPECT_EQ( Tracker::alive_count, 0 );

    ben::shared_ptr<Tracker> sp1( new Tracker( 30 ) );
    ben::shared_ptr<Tracker> sp2( new Tracker( 40 ) );

    EXPECT_EQ( Tracker::alive_count, 2 );

    sp1 = sp2;

    EXPECT_EQ( Tracker::alive_count, 1 ); 
    EXPECT_EQ( sp1->id, 40 );
    EXPECT_EQ( sp1.use_count(), 2 );
    
    sp1 = sp1;
    EXPECT_EQ( sp1.use_count(), 2 );
}

TEST( SharedPtrTest, WeakPtrLocking )
{
    EXPECT_EQ( Tracker::alive_count, 0 );
    
    ben::weak_ptr<Tracker> wp;
    {
        ben::shared_ptr<Tracker> sp( new Tracker( 50 ) );
        wp = sp;

        EXPECT_FALSE( wp.expired() );

        ben::shared_ptr<Tracker> locked = wp.lock();
        EXPECT_NE( locked.get(), nullptr );
        EXPECT_EQ( locked->id, 50 );
        EXPECT_EQ( locked.use_count(), 2 );
    }
    EXPECT_EQ( Tracker::alive_count, 0 );
    EXPECT_TRUE( wp.expired() );

    ben::shared_ptr<Tracker> fail = wp.lock();
    EXPECT_EQ( fail.get(), nullptr );
}

TEST( SharedPtrTest, CircularReferencePrevention )
{
    struct Node
    {
        ben::shared_ptr<Node> other;
        ben::weak_ptr<Node> weak_other;
    };

    ben::shared_ptr<Node> a( new Node() );
    ben::shared_ptr<Node> b( new Node() );

    a->other = b;
    b->weak_other = a;
}

int main ( int argc, char **argv )
{
    ::testing::InitGoogleTest( &argc, argv );
    return RUN_ALL_TESTS();
}
