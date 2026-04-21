#include <exception>
#include <iomanip>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

/*
 * REUSABLE OOP TEMPLATE (C++)
 * Replace placeholder names in UPPER_CASE with your problem-specific names.
 */

// ===============================
// 1) INTERFACE / ABSTRACT CLASS
// ===============================
class IPrintable {
public:
	virtual void printSummary() const = 0; // Abstraction + Interface
	virtual ~IPrintable() = default;
};

// =============================================
// 2) BASE CLASS (ENCAPSULATION + CONSTRUCTORS)
// =============================================
class BaseEntity : public IPrintable {
private:
	int id{};            // Encapsulated field
	string name;         // Encapsulated field

protected:
	// Protected helper for derived classes
	void validateName(const string& value) const {
		if (value.empty()) {
			throw invalid_argument("Name cannot be empty.");
		}
	}

public:
	// Default constructor
	BaseEntity() = default;

	// Parameterized constructor
	BaseEntity(int id, string name) : id(id), name(std::move(name)) {
		if (id <= 0) {
			throw invalid_argument("ID must be positive.");
		}
		validateName(this->name);
	}

	// Virtual destructor for polymorphic base class
	virtual ~BaseEntity() = default;

	// Getters / Setters (Encapsulation)
	int getId() const { return id; }
	const string& getName() const { return name; }

	void setId(int value) {
		if (value <= 0) {
			throw invalid_argument("ID must be positive.");
		}
		id = value;
	}

	void setName(const string& value) {
		validateName(value);
		name = value;
	}

	// Virtual method for overriding
	virtual double computeValue() const {
		return 0.0;
	}

	// Method overloading example
	void update(const string& newName) {
		setName(newName);
	}

	void update(int newId, const string& newName) {
		setId(newId);
		setName(newName);
	}

	// Interface method implementation
	void printSummary() const override {
		cout << "[BaseEntity] ID: " << id << ", Name: " << name << '\n';
	}
};

// ======================================================
// 3) COMPOSITION CLASS (HAS-A RELATIONSHIP)
// ======================================================
class Address {
private:
	string city;
	string state;

public:
	Address() = default;
	Address(string city, string state) : city(std::move(city)), state(std::move(state)) {}

	string toString() const {
		return city + ", " + state;
	}
};

// =================================================
// 4) DERIVED CLASS (INHERITANCE + OVERRIDING)
// =================================================
class DerivedEntity : public BaseEntity {
private:
	string category;
	double score{};
	Address address; // Composition: DerivedEntity owns Address

public:
	DerivedEntity() = default;

	DerivedEntity(int id, string name, string category, double score, Address address)
		: BaseEntity(id, std::move(name)),
		  category(std::move(category)),
		  score(score),
		  address(std::move(address)) {
		if (this->category.empty()) {
			throw invalid_argument("Category cannot be empty.");
		}
		if (this->score < 0.0) {
			throw invalid_argument("Score cannot be negative.");
		}
	}

	const string& getCategory() const { return category; }
	double getScore() const { return score; }

	void setCategory(const string& value) {
		if (value.empty()) {
			throw invalid_argument("Category cannot be empty.");
		}
		category = value;
	}

	void setScore(double value) {
		if (value < 0.0) {
			throw invalid_argument("Score cannot be negative.");
		}
		score = value;
	}

	// Overridden method (Polymorphism)
	double computeValue() const override {
		return score * 1.10;
	}

	// Overridden method from interface/base
	void printSummary() const override {
		cout << "[DerivedEntity] ID: " << getId()
			 << ", Name: " << getName()
			 << ", Category: " << category
			 << ", Score: " << fixed << setprecision(2) << score
			 << ", Address: " << address.toString() << '\n';
	}
};

// ========================================================
// 5) AGGREGATION CLASS (HAS REFERENCES TO EXTERNAL OBJECTS)
// ========================================================
class Manager {
private:
	vector<const BaseEntity*> items; // Aggregation: does not own the objects

public:
	void add(const BaseEntity& entity) {
		items.push_back(&entity);
	}

	void printAll() const {
		cout << "\n-- All Managed Entities --\n";
		for (const BaseEntity* item : items) {
			item->printSummary();
		}
	}
};

// ======================================
// 6) DRIVER CODE (MAIN / CLIENT USAGE)
// ======================================
int main() {
	try {
		// Collection / Array examples
		vector<unique_ptr<BaseEntity>> entities; // Polymorphic collection
		entities.push_back(make_unique<DerivedEntity>(
			1, "SAMPLE_NAME_1", "SAMPLE_CATEGORY_1", 88.5, Address("Pune", "MH")));
		entities.push_back(make_unique<DerivedEntity>(
			2, "SAMPLE_NAME_2", "SAMPLE_CATEGORY_2", 91.0, Address("Mumbai", "MH")));

		// Optional fixed-size array example (small practical tasks)
		int sampleArray[3] = {10, 20, 30};
		cout << "Array sample: ";
		for (int value : sampleArray) {
			cout << value << ' ';
		}
		cout << "\n";

		// Polymorphism: base pointer calling overridden functions
		cout << "\n-- Polymorphism Demo --\n";
		for (const auto& entity : entities) {
			entity->printSummary();
			cout << "Computed Value: " << fixed << setprecision(2)
				 << entity->computeValue() << "\n";
		}

		// Aggregation demo
		Manager manager;
		for (const auto& entity : entities) {
			manager.add(*entity);
		}
		manager.printAll();

		// Method overloading demo
		DerivedEntity temp(3, "TEMP", "TEMP_CATEGORY", 75.0, Address("Nashik", "MH"));
		temp.update("UPDATED_TEMP");
		temp.update(4, "UPDATED_TEMP_2");
		cout << "\nAfter update overloading calls:\n";
		temp.printSummary();
	} catch (const exception& ex) {
		// Exception handling example
		cerr << "Error: " << ex.what() << '\n';
	}

	return 0;
}
