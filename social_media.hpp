//
// Created by Yu Yang Zhang on 6/9/18.
//

#ifndef EOSIO_SOCIAL_MEDIA_HPP
#define EOSIO_SOCIAL_MEDIA_HPP

#include <eosiolib/eosio.hpp>
#include <eosio.system/eosio.system.hpp>
#include <eosio.token/eosio.token.hpp>

using namespace eosio;
using namespace std;

class social_media : public eosio::contract {
public:
	social_media(account_name self) : contract(self) {};

private:
	//@abi table
	struct account_info {
		account_name account;
		std::string avatar;
		std::string full_image;
		std::string website;
		std::string facebook;
		std::string twitter;
		std::string medium;
		std::string steemit;
		std::string telegram;
		std::string github;
		std::string organization;
		std::vector<string> follower_keys;

		auto primary_key() const { return account; }

		EOSLIB_SERIALIZE(account_info,
						 (account)(avatar)(full_image)(website)(facebook)(twitter)(medium)(steemit)(telegram)(github)(
								 organization)(follower_keys));
	};

	//@abi table
	struct pay_info {
		uint64_t pkey;
		uint32_t create_at;
		account_name payer;
		uint64_t filesize;
		asset pay_amount;

		auto primary_key() const { return pkey; }

		account_name get_payer() const { return payer; }

		EOSLIB_SERIALIZE(pay_info, (pkey)(create_at)(payer)(filesize)(pay_amount));
	};

	typedef eosio::multi_index<N(accountinfo), account_info> accountinfo;
	typedef eosio::multi_index<N(payinfo), pay_info,
			indexed_by<N(payer), const_mem_fun<pay_info, account_name, &pay_info::get_payer> >
	> payinfo;


public:
	//@abi action
	void setinfo(account_name account, string &avatar, string &full_image, string &website, string &facebook,
				 string &twitter, string &medium, string &steemit, string &telegram, string &github,
				 string &organization);

	//@abi action
	void regfollower(account_name followee, account_name follower, string &pub_key);

	//@abi action
	void payencrypt(account_name payer, uint64_t filesize);

	//@abi action
	void paydecrypt(account_name payer, uint64_t filesize);

};


#endif //EOSIO_SOCIAL_MEDIA_HPP
