class cas {
	public:
	int8_t sekundy, minuty, hodiny, dny, mesice;
	int16_t roky;
	int8_t * polak[5]{ &sekundy, &minuty, &hodiny, &dny, &mesice };
	cas() {
		sekundy = 0;
		minuty = 0;
		hodiny = 0;
		dny = 0;
		mesice = 0;
		roky = 0;
	}
	cas(uint8_t sekundi, uint8_t minuti, uint8_t hodnini) {
		sekundy = sekundi;
		minuty = minuti;
		hodiny = hodnini;
		dny = 0;
		mesice = 0;
		roky = 0;
	}
	cas(uint8_t sekundi, uint8_t minuti, uint8_t hodnini, uint8_t dni, uint8_t mesici, uint16_t roki) {
		sekundy = sekundi;
		minuty = minuti;
		hodiny = hodnini;
		dny = dni;
		mesice = mesici;
		roky = roki;
	}
	cas operator - (cas dalsi) {
		cas vysledny;
		if (sekundy - dalsi.sekundy < 0) {
			if (minuty - 1 < 0) {
				if (hodiny - 1 >= 0) {
					hodiny--;
					minuty += 59;
					sekundy += 60;
				}
			}
			else
			{
				minuty--;
				sekundy += 60;
			}
		}
		vysledny.sekundy = sekundy - dalsi.sekundy;
		if (minuty - 1 < 0) {
			if (hodiny - 1 >= 0) {
				hodiny--;
				minuty += 60;
			}
		}
		vysledny.minuty = minuty - dalsi.minuty;
		vysledny.hodiny = hodiny - dalsi.hodiny;
		return vysledny;
	}
	bool operator == (cas dalsi) {
		if (dny != 0 && dalsi.dny != 0) {
			if (sekundy == dalsi.sekundy&&minuty == dalsi.minuty&&hodiny == dalsi.hodiny&&dny == dalsi.dny&&mesice == dalsi.mesice&&roky == dalsi.roky) {
				return true;
			}
			return false;
		}
		else
		{
			if (sekundy == dalsi.sekundy&&minuty == dalsi.minuty&&hodiny == dalsi.hodiny) {
				return true;
			}
			return false;
		}
	}
	bool operator < (cas dalsi) {
		if (dny != 0 && dalsi.dny != 0) {
			if (roky < dalsi.roky) {
				return true;
			}
			else {
				if (roky == dalsi.roky) {
					for (int8_t i = 4; i >= 0; i--) {
						if (polak[i] < dalsi.polak[i]) {
							return true;
						}
						else {
							if (polak[i] == dalsi.polak[i]) {
								continue;
							}
							return false;
						}
					}
				}
				return false;
			}
		}
		else
		{
			for (int8_t i = 2; i >= 0; i--) {
				if (polak[i] < dalsi.polak[i]) {
					return true;
				}
				else {
					if (polak[i] == dalsi.polak[i]) {
						continue;
					}
					return false;
				}
			}
			return false;
		}
	}
	bool operator <= (cas dalsi) {
		if (dny != 0 && dalsi.dny != 0) {
			if (roky < dalsi.roky) {
				return true;
			}
			else {
				if (roky == dalsi.roky) {
					for (int8_t i = 4; i >= 0; i--) {
						if (polak[i] <= dalsi.polak[i]) {
							return true;
						}
						else {
							if (polak[i] == dalsi.polak[i]) {
								continue;
							}
							return false;
						}
					}
				}
				return true;
			}
		}
		else
		{
			for (int8_t i = 2; i >= 0; i--) {
				if (polak[i] <= dalsi.polak[i]) {
					return true;
				}
				else {
					if (polak[i] == dalsi.polak[i]) {
						continue;
					}
					return false;
				}
			}
			return true;
		}
	}
	bool operator > (cas dalsi) {
		if (dny != 0 && dalsi.dny != 0) {
			if (roky > dalsi.roky) {
				return true;
			}
			else {
				if (roky == dalsi.roky) {
					for (int8_t i = 4; i >= 0; i--) {
						if (polak[i] > dalsi.polak[i]) {
							return true;
						}
						else {
							if (polak[i] == dalsi.polak[i]) {
								continue;
							}
							return false;
						}
					}
				}
				return false;
			}
		}
		else
		{
			for (int8_t i = 2; i >= 0; i--) {
				if (polak[i] > dalsi.polak[i]) {
					return true;
				}
				else {
					if (polak[i] == dalsi.polak[i]) {
						continue;
					}
					return false;
				}
			}
			return false;
		}
	}
	bool operator >= (cas dalsi) {
		if (dny != 0 && dalsi.dny != 0) {
			if (roky >= dalsi.roky) {
				return true;
			}
			else {
				if (roky == dalsi.roky) {
					for (int8_t i = 4; i >= 0; i--) {
						if (polak[i] >= dalsi.polak[i]) {
							return true;
						}
						else {
							if (polak[i] == dalsi.polak[i]) {
								continue;
							}
							return false;
						}
					}
				}
				return true;
			}
		}
		else
		{
			for (int8_t i = 2; i >= 0; i--) {
				if (polak[i] >= dalsi.polak[i]) {
					return true;
				}
				else {
					if (polak[i] == dalsi.polak[i]) {
						continue;
					}
					return false;
				}
			}
			return true;
		}
	}
	bool operator != (cas dalsi) {
		if (dny != 0 && dalsi.dny != 0) {
			if (sekundy == dalsi.sekundy&&minuty == dalsi.minuty&&hodiny == dalsi.hodiny&&dny == dalsi.dny&&mesice == dalsi.mesice&&roky == dalsi.roky) {
				return false;
			}
			return true;
		}
		else
		{
			if (sekundy == dalsi.sekundy&&minuty == dalsi.minuty&&hodiny == dalsi.hodiny) {
				return false;
			}
			return true;
		}
	}
};