using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.InputSystem;

public class CamController : MonoBehaviour
{
	public InputAction rotate;
	public Vector2 sensitivity = Vector2.one * 5f;
	public float rotX = 0f, rotY = 0f;
	public Vector2 rotXBounds = Vector2.left * 80f + Vector2.up * 80f;
	public Transform head;

	Vector2 input = Vector2.zero;
	private void Awake() {
		rotate.started += ctx => {
			input = ctx.ReadValue<Vector2>();
			StartCoroutine(Rotate());
		};
		rotate.performed += ctx => {
			input = ctx.ReadValue<Vector2>();
		};
		rotate.canceled += ctx => {
			input = Vector2.zero;
		};

		Cursor.lockState = CursorLockMode.Locked;
	}

	private void OnEnable() {
		rotate.Enable();
	}

	private void OnDisable() {
		rotate.Disable();
	}

	IEnumerator Rotate() {
		while (input != Vector2.zero) {
			rotX = Mathf.Clamp(rotX + input.y * sensitivity.y * Time.deltaTime, rotXBounds.x, rotXBounds.y);
			rotY += input.x * sensitivity.x * Time.deltaTime;
			if (rotY < -200f)	rotY += 360f;
			if (rotY > 200f)		rotY -= 360f;
			transform.localRotation = Quaternion.Euler(0f, rotY, 0f);
			head.localRotation = Quaternion.Euler(rotX, 0f, 0f);
			yield return null;
		}
	}
}
