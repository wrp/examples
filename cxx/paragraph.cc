#include <iostream>
#include <vector>

class paragraph;

class option{
	public:
	std::string prompt;
	paragraph *destination;

	option(std::string p, paragraph* text){
		prompt = p;
		destination = text;
	}

	void display(){
		std::cout<<prompt;
		std::cout<<'\n';
	}
};

class paragraph{
	public:
	std::string text;
	std::vector <option> options;

	paragraph(std::string t)
	{text = t;}

	void display(){
		std::cout << text << '\n';
		for (option o:options){
			o.display();
		}
	}

};

int main(){
	paragraph intro("The trail comes to an end");
	paragraph death("Flowing lava swallows you whole");
	paragraph live("Light shine from the sky");
	intro.options.push_back(option("Die",&death));
	intro.display();
}
