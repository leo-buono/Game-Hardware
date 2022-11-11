using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.InputSystem;

public class PlayerController : MonoBehaviour
{
	public float rotSpeed = 360f;
	public float speed = 5f;
	public InputAction movement;
	public Transform meshBody;
	public Transform head;
	Rigidbody rb;

	bool moving = false;
	Vector2 input;
	float targetAngle = 0f;
	private void Awake() {
		rb = GetComponent<Rigidbody>();
		movement.started += ctx => {
			moving = true;
			input = ctx.ReadValue<Vector2>();
			StartCoroutine(Move());
		};
		movement.performed += ctx => {
			input = ctx.ReadValue<Vector2>();
		};
		movement.canceled += ctx => {
			input = Vector2.zero;
			moving = false;
		};
	}

	private void OnEnable() {
		movement.Enable();
	}

	private void OnDisable() {
		movement.Disable();
	}

	private void Update() {
		if (moving)
			targetAngle = head.rotation.eulerAngles.y - Vector2.SignedAngle(Vector2.up, input);

		if (targetAngle != meshBody.rotation.eulerAngles.y)
			meshBody.rotation = Quaternion.Euler(0f,
				Mathf.MoveTowardsAngle(meshBody.rotation.eulerAngles.y, targetAngle, rotSpeed * Time.deltaTime),0f);
	}

	WaitForFixedUpdate WFFU = new WaitForFixedUpdate();

	IEnumerator Move() {
		rb.velocity = head.rotation * new Vector3(input.x * speed, rb.velocity.y, input.y * speed);
		while (input != Vector2.zero) {
			yield return WFFU;
			rb.velocity = head.rotation * new Vector3(input.x * speed, rb.velocity.y, input.y * speed);
		}
	}
}
