#include <iostream>
#include <functional>
#include <boost/signals2.hpp>
#include <boost/smart_ptr.hpp>
#include <string>

#pragma region basic
void h(int code)
{
    std::cout << "Signal with value " << code << " received 3\n";
}

void basic()
{
    // T matches slot's type, including both return value and parameter
    boost::signals2::signal<void(int)> sig;

    // slot: ordinary funciton, functor, lambda
    auto f = [](int code)
    {
        std::cout << "Signal with value " << code << " received 1\n";
    };

    std::function<void(int)> g([](int code)
                               { std::cout << "Signal with value " << code << " received 2\n"; });

    // signal without connection
    sig(0);

    // signal with connection
    auto a = sig.connect(f);
    auto b = sig.connect(g);
    auto c = sig.connect(h);

    sig(1); // 1, 2, 3

    // disconnect a connection permanently
    c.disconnect();

    sig(2); // 1, 2

    {
        // a block comes into effect for connection b, temporarily
        boost::signals2::shared_connection_block block(b);
        sig(3); // 1
    }           // upon destruction, the block releases

    sig(4); // 1, 2
}
#pragma endregion

#pragma region customization
class Slot
{
public:
    void call()
    {
        std::cout << "slot\n";
    }

    void call2(int code)
    {
        std::cout << "slot received " << code << "\n";
    }
};

void customization()
{
    // priority
    // note the priority "values" actually denote groups
    // groups can have different slots in them
    boost::signals2::signal<void()> sig;
    auto a = sig.connect(1, []()
                         { std::cout << "1\n"; }); // higher value is higher priority
    auto b = sig.connect(1, []()
                         { std::cout << "0\n"; });
    sig();

    // scopped connector
    {
        // a scoped connection disconnects upon leaving scope, even if the origional connection is in the larger scope
        boost::signals2::scoped_connection sc(b);
        sig();
    }
    sig();

    // binding to member function using boost::bind
    // note boost::bind is NOT interchangeable with std::bind
    // _1 is used as standin for argument
    boost::signals2::signal<void(int)> sig2;
    Slot *slot = new Slot{};
    sig.connect(boost::bind(&Slot::call, slot));
    sig2.connect(boost::bind(&Slot::call2, slot, boost::placeholders::_1));

    sig();
    sig2(2);

    delete slot;

    // note, the slot still erroneously works after the pointer was deleted
    // (or it goes into segfault)
    // this is because the regular connection does not track deletion
    sig();
    sig2(2);

    sig.disconnect_all_slots();
    sig2.disconnect_all_slots();

    boost::signals2::signal<void(int)> sig3;
    // to track deletion, use shared pointer instead (note boost::shared_pointer instead of std::shared_pointer)
    {
        typedef boost::signals2::signal<void(int)> signal_type;
        auto slot2 = boost::make_shared<Slot>();

        sig3.connect(signal_type::slot_type(&Slot::call2, slot2.get(), boost::placeholders::_1).track(slot2));
    } // shared pointer is destroyed here

    sig3(3);
}

#pragma endregion

#pragma region advanced
class Callable
{
public:
    // use the class instance itself as a slot
    void operator()(int code)
    {
        std::cout << "called with " << code << "\n";
    }

    int increment(int code)
    {
        return code + 1;
    }
};

// referencing https://www.boost.org/doc/libs/1_79_0/doc/html/signals2/tutorial.html#id-1.3.35.4.5
// combiner which returns the maximum value returned by all slots
template <typename T>
struct maximum
{
    typedef T result_type;

    template <typename InputIterator>
    T operator()(InputIterator first, InputIterator last) const
    {
        // If there are no slots to call, just return the
        // default-constructed value
        if (first == last)
            return T();
        T max_value = *first++;
        while (first != last)
        {
            if (max_value < *first)
                max_value = *first;
            ++first;
        }

        return max_value;
    }
};

void advanced()
{
    boost::signals2::signal<int(int), maximum<int>> sig{};
    auto callable = boost::make_shared<Callable>();
    auto decrement = [](int code) -> int
    { return code - 1; };

    sig.connect(
        boost::signals2::signal<int(int)>::slot_type(
            &Callable::increment, callable.get(), boost::placeholders::_1)
            .track(callable));
    sig.connect(decrement);

    // sig(1) calls the maximum (callable struct) on the collection of return values
    auto res = sig(1);

    std::cout << "out of both slot returns, " << res << " is the maximum s" << std::endl;

    // referencing the connection from within the slot
    // instead of using T, use
    sig.connect_extended([](const boost::signals2::connection &conn, int value) -> int
                         {
        static int count = 0;
        if (++count > 3) conn.disconnect();
        else std::cout<<"this is call number "<<count<<"\n";

        return 0; });

    sig(1);
    sig(2);
    sig(3);
    auto fourth = sig(4); // note that the extended connection is not actually called because disconnect happened
    std::cout << "the other slots still ran, producing maximum " << fourth << "\n";
}

#pragma endregion

#pragma region notify_on_change

// T corresponds to owner class
template <typename T> class INotifyPropertyChanged {
public:
    boost::signals2::signal<void(const T&, std::string)> property_changed{};
};

class Notifiable : public INotifyPropertyChanged<Notifiable> {

public:

    // the tutorial also demonstrates using declspec() on a Windows platform to wrap the age variable
    // the effect is similar to the notify property changed interface

    int get_age() { return prop; }
    void set_age(int prop) { 
        if(prop == this->prop) return;
        property_changed(*this, "Age");
        this->prop = prop;
    }

private:
    int prop{};

};

void notify_on_change() {
    Notifiable n{};

    n.property_changed.connect([](const Notifiable& n, std::string s){
        std::cout<<s<<" has changed\n";
    });

    n.set_age(20);
}

#pragma endregion

int main(int argc, char *argv[])
{

    /**
     * @brief Signals v2
     * Relevant for event-based programming
     *   - observer pattern, binding
     * Supports signal-slot pattern instead of events
     */

    std::cout << "basic demo\n";
    basic();

    std::cout << "\n\ncustomization demo\n";
    customization();

    std::cout << "\n\nadvanced demo\n";
    advanced();

    std::cout << "\n\nnotify on change demo\n";
    notify_on_change();

    return 0;
}