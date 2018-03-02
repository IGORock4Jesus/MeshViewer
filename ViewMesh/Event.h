#pragma once



template <typename TOwner, typename ...TParameters>
class Event
{
	friend TOwner;

	enum class Type {
		Static,
		Class
	};

	struct Delegate
	{
		virtual void Process(TParameters... parameters) = 0;
		virtual Type GetType() = 0;
	};

	template <typename TFucntion>
	struct StaticDelegate : Delegate
	{
		TFucntion function;

		virtual void Process(TParameters... parameters) override {
			function(parameters...);
		}

		virtual Type GetType() override { return Type::Static; }
	};

	template <typename TClass, typename TMethod>
	struct ClassDelegate : Delegate
	{
		TClass* object;
		TMethod method;

		virtual void Process(TParameters... parameters) override
		{
			(object->*method)(parameters...);
		}

		virtual Type GetType() override { return Type::Class; }
	};

	std::list<std::shared_ptr<Delegate>> _delegates;


	void operator () (TParameters... parameters)
	{
		for (auto&& p : _delegates) {
			p->Process(parameters...);
		}
	}
public:

	template <typename TFunction>
	void Add(TFunction function)
	{
		auto p = new StaticDelegate<TFunction>();
		p->function = function;
		_delegates.push_back(std::shared_ptr<Delegate>(p));
	}

	template <typename TClass, typename TMethod>
	void Add(TClass* object, TMethod method)
	{
		auto p = new ClassDelegate<TClass, TMethod>();
		p->object = object;
		p->method = method;
		_delegates.push_back(std::shared_ptr<Delegate>(p));
	}

	template <typename TFunction>
	void Remove(TFunction function)
	{
		_delegates.remove_if([&function](std::shared_ptr<Delegate> c)
		{
			if (c->GetType() == Type::Static)
			{
				if (((StaticDelegate<TFunction>*)c.get())->function == function)
				{
					return true;
				}
			}
			return false;
		});
	}

	template <typename TClass, typename TMethod>
	void Remove(TClass* object, TMethod method) {
		_delegates.remove_if([object, method](std::shared_ptr<Delegate> c)
		{
			if (c->GetType() == Type::Class)
			{
				auto p = (ClassDelegate<TClass, TMethod>*)c.get();

				if (p->object == object && p->method == method)
				{
					return true;
				}
			}
			return false;
		});
	}

	template <typename TFunction>
	void operator += (TFunction function) {
		Add<TFunction>(function);
	}
	template <typename TClass, typename TMethod>
	void operator += (const std::pair<TClass*, TMethod>& object) {
		Add(object.first, object.second);
	}

	template <typename TFunction>
	void operator -= (TFunction function) {
		Remove(function);
	}
	template <typename TClass, typename TMethod>
	void operator -= (const std::pair<TClass*, TMethod>& object) {
		Remove(object.first, object.second);
	}
};

