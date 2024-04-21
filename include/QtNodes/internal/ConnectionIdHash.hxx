///
/// @file ConnectionIdHash.hxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-17
/// @copyright Copyright (c) 2024
///

#ifndef CONNECTION_ID_HASH_HXX_44201234E15B
#define CONNECTION_ID_HASH_HXX_44201234E15B

#include <functional>

#include "Definitions.hxx"

///
/// @brief
/// @param seed
///
inline void hash_combine(std::size_t& seed)
{
    Q_UNUSED(seed);
}

///
/// @brief
/// @tparam T
/// @tparam ...Rest
/// @param seed
/// @param v
/// @param ...rest
///
template<typename T, typename... Rest>
inline void hash_combine(std::size_t& seed, const T& v, Rest... rest)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e'37'79'b9 + (seed << 6) + (seed >> 2);
    hash_combine(seed, rest...);
}

namespace std {

///
/// @brief
///
/// @tparam
///
template<>
struct hash<QtNodes::ConnectionId>
{
    inline std::size_t operator()(const QtNodes::ConnectionId& id) const
    {
        std::size_t h = 0;
        hash_combine(
            h,
            id.outNodeId,
            id.outPortIndex,
            id.inNodeId,
            id.inPortIndex
        );
        return h;
    }
};

///
/// @brief std::hash<std::pair<QtNodes::NodeId, QtNodes::PortIndex>>
///
template<>
struct hash<std::pair<QtNodes::NodeId, QtNodes::PortIndex>>
{
    inline std::size_t operator()(
        const std::pair<QtNodes::NodeId, QtNodes::PortIndex>& nodePort
    ) const
    {
        std::size_t h = 0;
        hash_combine(h, nodePort.first, nodePort.second);
        return h;
    }
};

///
/// @brief std::hash<std::tuple<QtNodes::NodeId, QtNodes::PortType, QtNodes::PortIndex>>
///
template<>
struct hash<std::tuple<QtNodes::NodeId, QtNodes::PortType, QtNodes::PortIndex>>
{
    using Key
        = std::tuple<QtNodes::NodeId, QtNodes::PortType, QtNodes::PortIndex>;

    inline std::size_t operator()(const Key& key) const
    {
        std::size_t h = 0;
        hash_combine(h, std::get<0>(key), std::get<1>(key), std::get<2>(key));
        return h;
    }
};

}  // namespace std

#endif  // CONNECTION_ID_HASH_HXX_44201234E15B
