#pragma once

namespace sb
{
	template <class T>
	inline void copy(T* destination, const T* source) {
		destination = NULL;
		if (source)
			destination = new T(*source);
	}

	template <class T>
	inline void copyVector(std::vector<T*>& destination, const std::vector<T*>& source) {
		destination.clear();
		for (std::size_t i = 0; i < source.size(); i++) {
			T* element = NULL;
			copy(element, source[i]);
			destination.push_back(element);
		}

		std::copy(source.begin(), source.end(), destination.begin());
	}

	template <class T>
	inline void deleteFromVector(std::vector<T*>& vec, bool(*predicate)(T*)) {
		std::vector<T*> toDelete;
		for (std::size_t i = 0; i < vec.size(); i++) {
			if (predicate(vec[i]))
				toDelete.push_back(vec[i]);
		}

		vec.erase(std::remove_if(vec.begin(), vec.end(), predicate), vec.end());

		for (std::size_t i = 0; i < toDelete.size(); i++)
			delete toDelete[i];
	}

	template <class T>
	void copyAll(const T& source, T& destination)
	{
		std::copy(std::begin(source), std::end(source), std::begin(destination));
	}

	template <class T> 
	void deleteAll(std::vector<T>& vec) {
		for (size_t i = 0; i < vec.size(); i++)
			delete vec[i];
	}

	template <class T, class Predicate>
	typename std::vector<T>::iterator remove_if(std::vector<T>& vec, const Predicate& predicate) {
		return std::remove_if(vec.begin(), vec.end(), predicate);
	}

	template <class T>
	struct Find {
		const std::vector<T>& vec;

		Find(const std::vector<T>& vec_) : vec(vec_) { }

		bool operator()(const T& t) const {
			return std::find(vec.begin(), vec.end(), t) != vec.end();
		}
	};

	template <class T>
	void erase(std::vector<T>& v, std::vector<T>& toRemove) {
		v.erase(remove_if(v, Find<T>(toRemove)), v.end());
	}
	
	//template <class T, class Predicate>
	//void remove_if(std::vector<T>& vec, const Predicate& predicate) {
	//	std::remove_if(vec.begin(), vec.end(), predicate);
	//}

	//template <class T>
	//struct Find {
	//	const std::vector<T>& vec;

	//	Find(const std::vector<T>& vec_) : vec(vec_) { }

	//	bool operator()(const T& t) const {
	//		return std::find(vec.begin(), vec.end(), t) != vec.end();
	//	}
	//};

	//template <class T>
	//void erase(std::vector<T>& vec, const std::vector<T>& toRemove) {
	//	remove_if(vec, Find<T>(toRemove));
	//}
}