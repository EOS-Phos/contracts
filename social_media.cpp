//
// Created by Yu Yang Zhang on 6/9/18.
//

#include "social_media.hpp"

void social_media::setinfo(account_name account, string &avatar, string &full_image, string &website, string &facebook,
						   string &twitter, string &medium, string &steemit, string &telegram, string &github,
						   string &organization) {
	require_auth(account);
	eosio::print("account is ", name{account}, "\n");

	social_media::accountinfo accountinfo(_self, account);

	auto itr = accountinfo.find(account);

	if (itr == accountinfo.end()) {
		accountinfo.emplace(account, [&](auto &p) {
			p.account = account;
			p.avatar = avatar;
			p.full_image = full_image;
			p.website = website;
			p.facebook = facebook;
			p.twitter = twitter;
			p.medium = medium;
			p.steemit = steemit;
			p.telegram = telegram;
			p.github = github;
			p.organization = organization;
			print("new organization is ", p.organization, "\n");
		});
	} else {
		accountinfo.modify(itr, itr->account, [&](auto &p) {
			print("origin organization is ", p.organization, "\n");
			p.account = account;
			p.avatar = avatar;
			p.full_image = full_image;
			p.website = website;
			p.facebook = facebook;
			p.twitter = twitter;
			p.medium = medium;
			p.steemit = steemit;
			p.telegram = telegram;
			p.github = github;
			p.organization = organization;

			print("update organization is ", p.organization, "\n");
		});
	}
}

void social_media::regfollower(account_name followee, account_name follower, string &pub_key) {

	require_auth(follower);

	social_media::accountinfo accountinfo(_self, followee);

	auto itr = accountinfo.find(followee);
	eosio_assert(itr != accountinfo.end(), "you can't follow a non exist account");
//	print("follower is ", itr->account);

	accountinfo.modify(itr, 0, [&](auto &p) {
		p.follower_keys.push_back(pub_key);
	});
}

void social_media::payencrypt(account_name payer, uint64_t filesize) {

	require_auth(payer);

	social_media::payinfo payinfo(_self, _self);
	auto payer_index = payinfo.get_index<N(payer)>();

	auto itr = payer_index.find(payer);
	uint64_t amount = filesize * 0.01;
	payinfo.emplace(payer, [&](auto &p) {
		p.pkey = payinfo.available_primary_key();
		p.create_at = now();
		p.payer = payer;
		p.filesize = filesize;
		p.pay_amount = asset(amount);
	});

	INLINE_ACTION_SENDER(eosio::token, transfer)(N(eosio.token), {payer, N(active)},
												 {payer, N(socialmedian), asset(amount),
												  std::string("encrypt fee")});

}

void social_media::paydecrypt(account_name payer, uint64_t filesize) {

	require_auth(payer);
	social_media::payinfo payinfo(_self, _self);
	auto payer_index = payinfo.get_index<N(payer)>();

	auto itr = payer_index.find(payer);
	uint64_t amount = filesize * 0.01;
	payinfo.emplace(payer, [&](auto &p) {
		p.pkey = payinfo.available_primary_key();
		p.create_at = now();
		p.payer = payer;
		p.filesize = filesize;
		p.pay_amount = asset(amount);
	});

	INLINE_ACTION_SENDER(eosio::token, transfer)(N(eosio.token), {payer, N(active)},
												 {payer, N(socialmedian), asset(amount),
												  std::string("decryption fee")});
}


EOSIO_ABI(social_media, (setinfo)(regfollower)(payencrypt)(paydecrypt))