
// my github: https://github.com/bossiskar

#include <Windows.h>
#include "generate.h"
#include "checker.h"


HANDLE hConsole = NULL;

int main(int argc, char** argv)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTitleA("[Seed phrase Generate & Check - BTC; ETH; LTC; DOGE] - By Cosmo11");

	DWORD webstat = 0;
	if(!inet::webstatus_check("https://www.blockchain.com/"))
	{
		std::cout << _("Failed to access the blockchain. Check your internet connection\n");
		Sleep(5000);
		exit(0);
	}

	srand(static_cast<unsigned int>(time(0)));
	DWORD64 seed_count = 0;
	float total_balance = 0;

menu:
	std::cout << _("select an action:\n'1' - generate 1 seed phrase\n'2' - search for seed phrases with coins(BTC, ETH, LTC, DOGE)\n");
	while (true) {
		if (GetAsyncKeyState('1') & 1) {
			std::cout << "\n\n";
			std::string seed = generate_seed_phrase(12);
			std::cout << _("seed: ") << seed << "\n\n";
			goto menu;
		}
		else if (GetAsyncKeyState(('2')) & 1) {
			goto brute;
		}
		Sleep(1);
	}

brute:
	while (true) {
		std::string seed = generate_seed_phrase(12);
		std::cout << _("seed: ") << seed;
		
		balance wallet_balance;
		if (check_wallet(seed, &wallet_balance) != 0)
			continue;

		if (is_empty(balance)) {
			SetConsoleTextAttribute(hConsole, 4);
			std::cout << _(" (empty)");
			SetConsoleTextAttribute(hConsole, 7);
		}
		else {
			SetConsoleTextAttribute(hConsole, 2);
			std::cout << _(" (with balance)");
			SetConsoleTextAttribute(hConsole, 7);

			total_balance += balance.btc * get_btc_price();
			total_balance += balance.eth * get_eth_price();
			total_balance += balance.doge * get_doge_price();
			total_balance += balance.ltc * get_ltc_price();

			std::string found_info = "address: " + get_wallet_address_from_mnemonic() + "\nmnemonic: " + seed + "\nprivate key: " + 
				get_private_key_from_mnemonic() + "\nbanace: " + balance.btc + "BTC " + balance.eth + "ETH " + balance.doge + "DOGE " + balance.ltc + "LTC\n\n";
			HANDLE hfile = CreateFileA("found_wallets.txt", FILE_ALL_ACCESS, NULL, nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			WriteFile(hfile, found_info.c_str(), found_info.size(), nullptr, nullptr);
			CloseHandle(hfile);
		}

		std::cout << std::endl;
		std::string tittle = info + _as(" | ") + _as("Checked seeds: ") + std::to_string(seed_count) + _as(" | Total balance: $") + std::to_string(total_balance);
		SetConsoleTitleA(tittle.c_str());
		++seed_count;

		if ((seed_count % 10000000) == 0) {
			system(_("cls"));
		}
	}

	return 0;
}
