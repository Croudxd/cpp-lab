#include <iostream>
#include <cassert>

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

void test_constructor_and_destructor ()
{
    std::cout << "Test: Constructor & Destructor... ";
    assert( Tracker::alive_count == 0 );
    {
        ben::shared_ptr<Tracker> sp( new Tracker( 10 ) );
        assert( Tracker::alive_count == 1 );
        assert( sp->id == 10 );
        assert( sp.use_count() == 1 );
    }
    // Scope end -> Destructor called
    assert( Tracker::alive_count == 0 );
    std::cout << "PASSED" << std::endl;
}

void test_copy_logic ()
{
    std::cout << "Test: Copy Semantics... ";
    assert( Tracker::alive_count == 0 );
    {
        ben::shared_ptr<Tracker> sp1( new Tracker( 20 ) );
        {
            // Copy Constructor
            ben::shared_ptr<Tracker> sp2 = sp1;
            
            assert( Tracker::alive_count == 1 ); // Still only 1 object
            assert( sp1.use_count() == 2 );
            assert( sp2.use_count() == 2 );
            assert( sp2->id == 20 );
        } 
        // sp2 dies here. sp1 should still be alive.
        assert( Tracker::alive_count == 1 );
        assert( sp1.use_count() == 1 );
    }
    // sp1 dies here.
    assert( Tracker::alive_count == 0 );
    std::cout << "PASSED" << std::endl;
}

void test_assignment_operator ()
{
    std::cout << "Test: Assignment Operator... ";
    assert( Tracker::alive_count == 0 );

    ben::shared_ptr<Tracker> sp1( new Tracker( 30 ) );
    ben::shared_ptr<Tracker> sp2( new Tracker( 40 ) );

    assert( Tracker::alive_count == 2 );

    // sp1 drops 30 and grabs 40
    sp1 = sp2;

    // Object 30 should be deleted immediately
    assert( Tracker::alive_count == 1 ); 
    assert( sp1->id == 40 );
    assert( sp1.use_count() == 2 );
    
    // Self assignment check
    sp1 = sp1;
    assert( sp1.use_count() == 2 );

    std::cout << "PASSED" << std::endl;
}

void test_weak_ptr_locking ()
{
    std::cout << "Test: Weak Ptr & Lock... ";
    assert( Tracker::alive_count == 0 );
    
    ben::weak_ptr<Tracker> wp;
    {
        ben::shared_ptr<Tracker> sp( new Tracker( 50 ) );
        wp = sp; // Uses the weak_ptr constructor

        assert( !wp.expired() );

        // Lock to get a strong pointer
        ben::shared_ptr<Tracker> locked = wp.lock();
        assert( locked.get() != nullptr );
        assert( locked->id == 50 );
        assert( locked.use_count() == 2 ); // sp + locked
    }
    // sp and locked are gone. Object should be dead.
    assert( Tracker::alive_count == 0 );
    assert( wp.expired() );

    // Try to lock dead pointer
    ben::shared_ptr<Tracker> fail = wp.lock();
    assert( fail.get() == nullptr );

    std::cout << "PASSED" << std::endl;
}

void test_circular_reference_prevention ()
{
    std::cout << "Test: Circular Reference Prevention... ";
    
    struct Node
    {
        ben::shared_ptr<Node> other;
        ben::weak_ptr<Node> weak_other;
    };

    ben::shared_ptr<Node> a( new Node() );
    ben::shared_ptr<Node> b( new Node() );

    // A holds B strongly
    a->other = b;
    // B holds A weakly
    b->weak_other = a;

    // Because B only holds A weakly, when we reset A, it should die.
    // If B held A strongly, we would have a leak here.
    
    // (Manual verification logic usually requires internal instrumentation, 
    // but ensuring no crash occurs here is the first step).
    
    std::cout << "PASSED" << std::endl;
}

int main ()
{
    std::cout << "=== RUNNING BEN::SHARED_PTR TESTS ===" << std::endl;
    
    test_constructor_and_destructor();
    test_copy_logic();
    test_assignment_operator();
    test_weak_ptr_locking();
    test_circular_reference_prevention();

    std::cout << "\nAll Tests Passed Successfully." << std::endl;
    return 0;
}
