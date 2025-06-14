// SPDX-License-Identifier: MIT
pragma solidity ^0.8.0;

import "./IERC721.sol";

contract VulnerablePool is IERC721Receiver {
    mapping(address => uint256) public balances;
    mapping(uint256 => bool) public depositedTokens;

    function depositNFT(address nftAddress, uint256 tokenId) external {
        IERC721(nftAddress).safeTransferFrom(msg.sender, address(this), tokenId);
        require(IERC721(nftAddress).ownerOf(tokenId) == address(this), "Transfer failed");

        if (!depositedTokens[tokenId]) {
            balances[msg.sender] += 1e18;
            depositedTokens[tokenId] = true;
        }
    }

    function withdraw() external {
        uint256 amount = balances[msg.sender];
        require(amount > 0, "No balance");
        balances[msg.sender] = 0;
        payable(msg.sender).transfer(amount);
    }

    function onERC721Received(address, address, uint256, bytes calldata) external pure override returns (bytes4) {
        return IERC721Receiver.onERC721Received.selector;
    }
}