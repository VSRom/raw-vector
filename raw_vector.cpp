
//===============================================================================================================================
int main() {
	{
		raw_vector<int> hello = { 748, 21, 15 ,23, 14 };
		raw_vector<int> copy;


		//	copy = hello;
		//	hello = hello;

		//	for (const auto& x : hello)
		//		std::cout << x << '\n';
		int a = 420;
		hello.resize(25);
		//std::cout << copy << '\n';
		std::cout << hello << '\n';
	}
	std::cout << sizeof(raw_vector<int>) << " == " << sizeof(int*) << "\n\n\n";



	return 0;
}
//===============================================================================================================================
