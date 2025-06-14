// SPDX-License-Identifier: MIT
pragma solidity ^0.7.6;

contract VulnerableToken {
    mapping(address => uint256) public balances;

    function addBalance() external payable {
        require(msg.value == 0.001 ether, "You must send exactly 0.001 ETH");
        balances[msg.sender] += 1000; 
    }

    function getBalance(address user) external view returns (uint256) {
        return balances[user];
    }

    function setBalance(uint256 newBalance) external {
        balances[msg.sender] = newBalance;
    }
}