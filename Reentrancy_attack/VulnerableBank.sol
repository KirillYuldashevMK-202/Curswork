// VULNERABLE BANK SMART CONTRACT

// SPDX-License-Identifier: MIT
pragma solidity ^0.8.26;

import "@openzeppelin/contracts/utils/Address.sol";
import "hardhat/console.sol";

contract EtherBank{
    using Address for address payable;

    mapping(address => uint) public balances;

    function deposit() external payable {
        balances[msg.sender] += msg.value;
    }

    function withdraw() external{
        require(balances[msg.sender] > 0, "Withdrawl amount exceeds available balance.");
        
        console.log("");
        console.log("EtherBank balance: ", address(this).balance);
        console.log("Attacker balance: ", balances[msg.sender]);
        console.log("");

        payable(msg.sender).sendValue(balances[msg.sender]);
        balances[msg.sender] = 0;
    }

    function getBalance() external view returns (uint) {
        return address(this).balance;
    }
}