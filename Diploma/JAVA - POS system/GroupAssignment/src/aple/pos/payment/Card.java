/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package aple.pos.payment;

/**
 *
 * @author user
 */
public class Card{
    private String bank;
    private String type;
    private String cardNo;
    private float balance;

    public Card(String bank, String type, String cardNo, float balance) {
        this.bank = bank;
        this.type = type;
        this.cardNo = cardNo;
        this.balance = balance;
    }

    public String getBank() {
        return bank;
    }

    public void setBank(String bank) {
        this.bank = bank;
    }

    public String getType() {
        return type;
    }

    public void setType(String type) {
        this.type = type;
    }

    public String getCardNo() {
        return cardNo;
    }

    public void setCardNo(String cardNo) {
        this.cardNo = cardNo;
    }

    public float getBalance() {
        return balance;
    }

    public void setBalance(float balance) {
        this.balance = balance;
    }   
}