#ifndef SERIALIZETUPLE_H
#define SERIALIZETUPLE_H

#include <tuple>

namespace boost {
namespace serialization {

// Serialize std::tuple<int, int>.
struct SerializeTuple
{
    template<class Archive>
    static void serialize(Archive & ar, std::tuple<int, int> & t, const unsigned int version)
    {
        ar & std::get<0>(t);
        ar & std::get<1>(t);
    }
};

template<class Archive>
void serialize(Archive & ar, std::tuple<int, int> & t, const unsigned int version)
{
    // This will exclusively and non-recursively serialize std::tuple<int, int>.
    SerializeTuple::serialize(ar, t, version);
}

}
}

#endif // SERIALIZETUPLE_H
