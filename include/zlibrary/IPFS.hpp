//
// Created by Advik on 07-04-2025.
//

#ifndef IPFS_HPP
#define IPFS_HPP

#include <zlibrary/Export.hpp>
#include <string>
namespace zlibrary {

    struct ZLIBRARY_API IPFS {
        const std::string cid;
        const std::string cid_blake2b;
    };

} // namespace zlibrary

#endif //IPFS_HPP
