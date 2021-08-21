#ifndef SERIALIZETUPLERECURSIVE_H
#define SERIALIZETUPLERECURSIVE_H

#include <tuple>

namespace boost {
namespace serialization {

// Serialize std::tuple<Args...> recursively.
template<unsigned int N>
struct SerializeTupleRecursive
{
    template<class Archive, typename... Args>
    static void serialize(Archive & ar, std::tuple<Args...> & t, const unsigned int version)
    {
        ar & std::get<N-1>(t);
        SerializeTupleRecursive<N-1>::serialize(ar, t, version);
    }
};

// explicit specialization must be preceded by 'template <>.
template<>
struct SerializeTupleRecursive<0>
{
    template<class Archive, typename... Args>
    static void serialize(Archive & ar, std::tuple<Args...> & t, const unsigned int version)
    {
        (void) ar;
        (void) t;
        (void) version;
    }
};

// Use parameter pack "... Args" for std::tuple<Args...> (e.g., std::tuple<int, int>) as template.
template<class Archive, typename... Args>
void serialize(Archive & ar, std::tuple<Args...> & t, const unsigned int version)
{
    // This will recursively serialize std::tuple<Args...>, where "sizeof...(Args)" is number of arguments of std::tuple<Args...> to be serialized.
    SerializeTupleRecursive<sizeof...(Args)>::serialize(ar, t, version);
}

}
}

#endif // SERIALIZETUPLERECURSIVE_H
